#include "pch.h"

#include "GridBroadPhaseSystem.h"

#include "Engine/ECS/Components.h"
#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/SingletonComponents/PhysicsLocator.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Utils/GridUtils.h"
#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
	typedef std::set< Entity >::iterator itEntities;
	typedef std::map< uint /*index*/, GridAABB* >::iterator itIdxAABB;
	typedef std::pair< uint /*index*/, GridAABB* > pairIdxAABB;

	void GridBroadPhaseSystem::Init()
	{
	}

	void GridBroadPhaseSystem::Start(std::shared_ptr<Scene> pScene)
	{
		m_UpdateRigidBodies(pScene);
	}

	void GridBroadPhaseSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();
		std::shared_ptr<NarrowPhaseTestsComponent> pNarrowTests = PhysicsLocator::GetNarrowPhaseTests();

		// Clear active entities first
		for (itIdxAABB it = pGrid->mapAABBs.begin(); it != pGrid->mapAABBs.end(); ++it)
		{
			int key = it->first;
			GridAABB* pAABB = it->second;

			pAABB->vecActiveEntities.clear();
			pAABB->vecPassiveEntities.clear();
		}

		// Clear all test groups
		pNarrowTests->staticEntitiesToTest.clear();
		pNarrowTests->passiveEntitiesToTest.clear();
		pNarrowTests->activeEntitiesToTest.clear();

		// Update aabbs active and passive entities positions
		m_UpdateRigidBodies(pScene, true);

		// Update testing groups for narrow phase
		int i = -1;
		for (itIdxAABB it = pGrid->mapAABBs.begin(); it != pGrid->mapAABBs.end();)
		{
			GridAABB* pAABB = it->second;

			// Only add to narrow phase testing groups if we have active entity on aabb
			if (pAABB->vecActiveEntities.size() > 0)
			{
				pNarrowTests->staticEntitiesToTest.push_back({});
				pNarrowTests->passiveEntitiesToTest.push_back({});
				pNarrowTests->activeEntitiesToTest.push_back({});

				i++;

				for (Entity entityId : pAABB->vecActiveEntities)
				{
					pNarrowTests->activeEntitiesToTest[i].push_back(entityId);
				}

				for (Entity entityId : pAABB->vecStaticEntities)
				{
					pNarrowTests->staticEntitiesToTest[i].push_back(entityId);
				}

				for (Entity entityId : pAABB->vecPassiveEntities)
				{
					pNarrowTests->passiveEntitiesToTest[i].push_back(entityId);
				}
			}

			// Check if aabb is empty to remove from mapping
			if (pAABB->Total() == 0)
			{
				delete pAABB;
				it = pGrid->mapAABBs.erase(it);
			}
			else
			{
				++it;
			}
		}
	}

	void GridBroadPhaseSystem::Render(std::shared_ptr<Scene> pScene)
	{
	}

	void GridBroadPhaseSystem::End(std::shared_ptr<Scene> pScene)
	{
		m_ClearAABBs();
	}

	void GridBroadPhaseSystem::Shutdown()
	{
	}

	GridAABB* GridBroadPhaseSystem::m_GetAABB(uint idxAABB)
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		itIdxAABB itAABB = pGrid->mapAABBs.find(idxAABB);
		if (itAABB == pGrid->mapAABBs.end())
		{
			return nullptr;
		}

		return itAABB->second;
	}

	GridAABB* GridBroadPhaseSystem::m_GetAABB(const glm::vec3& point)
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		uint idxAABB = GridUtils::LocatePoint(point, pGrid->lengthPerBox);

		return m_GetAABB(idxAABB);
	}

	GridAABB* GridBroadPhaseSystem::m_GetOrCreateAABB(uint idxAABB)
	{
		GridAABB* pAABB = m_GetAABB(idxAABB);
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		if (pAABB)
		{
			return pAABB;
		}

		pAABB = new GridAABB();
		pAABB->minXYZ = GridUtils::LocatePosition(idxAABB, pGrid->lengthPerBox);
		pAABB->maxXYZ = pAABB->minXYZ + pGrid->lengthPerBox;

		pGrid->mapAABBs[idxAABB] = pAABB;

		return pAABB;
	}

	void GridBroadPhaseSystem::m_UpdateRigidBodies(std::shared_ptr<Scene> pScene, bool updatePassive)
	{
		// Creating AABBs grid
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		// Rigidbody entities
		for (Entity entityId : SceneView<TransformComponent, RigidBodyComponent>(*pScene))
		{
			TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
			RigidBodyComponent& rigidBody = pScene->Get<RigidBodyComponent>(entityId);

			if (!updatePassive && rigidBody.bodyType == eBody::STATIC)
			{
				continue;
			}

			uint idxpos = GridUtils::LocatePoint(transform.worldPosition, pGrid->lengthPerBox);
			m_InsertEntity(entityId, idxpos, rigidBody.bodyType);

			if (rigidBody.shapeType == eShape::SPHERE)
			{
				SphereColliderComponent& sphere = pScene->Get<SphereColliderComponent>(entityId);
				m_InsertSphere(entityId, idxpos, transform.position,
					sphere.radius, rigidBody.bodyType, pGrid);
			}
			else
			{
				LOG_WARNING("Shape type not implemented yet: " + std::to_string(rigidBody.shapeType));
			}
		}
	}

	void GridBroadPhaseSystem::m_InsertSphere(Entity entityID, uint originIndex,
											  const glm::vec3& position, float radius, 
											  const eBody& bodyType,
											  std::shared_ptr<GridBroadphaseComponent> pGrid)
	{
		// Check collisions in the neighboring cells
		for (int i = -1; i <= 1; ++i)
		{
			float radiusI = radius * i;
			for (int j = -1; j <= 1; ++j)
			{
				float radiusJ = radius * j;
				for (int k = -1; k <= 1; k++)
				{
					if (j == 0 && i == 0 && k == 0)
					{
						// Same aabb
						continue;
					}

					float radiusK = radius * k;

					glm::vec3 currRadius = glm::vec3(radiusI, radiusJ, radiusK);
					glm::vec3 currPoint = position + currRadius;

					uint currIdxpos = GridUtils::LocatePoint(currPoint, pGrid->lengthPerBox);

					if (currIdxpos == originIndex)
					{
						// Same aabb
						continue;
					}

					m_InsertEntity(entityID, currIdxpos, bodyType);
				}
			}
		}
	}

	void GridBroadPhaseSystem::m_InsertEntity(Entity entityID, uint index, const eBody& bodyType)
	{
		GridAABB* pAABB = m_GetOrCreateAABB(index);

		if (bodyType == eBody::STATIC)
		{
			pAABB->vecStaticEntities.insert(entityID);
		}
		else if (bodyType == eBody::PASSIVE)
		{
			pAABB->vecPassiveEntities.insert(entityID);
		}
		else
		{
			pAABB->vecActiveEntities.insert(entityID);
		}

		return;
	}

	void GridBroadPhaseSystem::m_ClearAABBs()
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		for (pairIdxAABB pairAABB : pGrid->mapAABBs)
		{
			delete pairAABB.second;
		}

		pGrid->mapAABBs.clear();
	}

	size_t GridBroadPhaseSystem::m_RemoveAABB(uint idxAABB)
	{
		GridAABB* pAABB = m_GetAABB(idxAABB);
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		if (!pAABB)
		{
			return 0;
		}

		size_t left = pGrid->mapAABBs.erase(idxAABB);
		delete pAABB;

		return left;
	}
}
