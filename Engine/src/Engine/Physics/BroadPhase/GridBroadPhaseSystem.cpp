#include "pch.h"

#include "GridBroadPhaseSystem.h"

#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/GridUtils.h"
#include "Engine/Utils/TransformUtils.h"

#include "Engine/Physics/Components/Components.h"
#include "Engine/Physics/Components/PhysicsLocator.h"

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
		EntitySystem::Start(pScene);

		m_UpdateRigidBodies(pScene);
	}

	void GridBroadPhaseSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();
		std::shared_ptr<NarrowPhaseTestsComponent> pNarrowTests = PhysicsLocator::GetNarrowPhaseTests();

		// Clear active entities first
		pGrid->LockWrite();
		for (itIdxAABB it = pGrid->mapAABBs.begin(); it != pGrid->mapAABBs.end(); ++it)
		{
			int key = it->first;
			GridAABB* pAABB = it->second;

			pAABB->vecActiveEntities.clear();
			pAABB->vecPassiveEntities.clear();
		}
		pGrid->UnlockWrite();

		// Clear all test groups
		pNarrowTests->LockWrite();
		pNarrowTests->staticEntitiesToTest.clear();
		pNarrowTests->passiveEntitiesToTest.clear();
		pNarrowTests->activeEntitiesToTest.clear();
		pNarrowTests->UnlockWrite();

		// Update aabbs active and passive entities positions
		m_UpdateRigidBodies(pScene, true);

		// Update testing groups for narrow phase
		int i = -1;
		pGrid->LockWrite();
		pNarrowTests->LockWrite();
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
		pNarrowTests->UnlockWrite();
		pGrid->UnlockWrite();
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

	void GridBroadPhaseSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
	{
		ComponentType transformType = pScene->GetComponentType<TransformComponent>();
		ComponentType rigidbodyType = pScene->GetComponentType<RigidBodyComponent>();

		m_systemMask.set(transformType);
		m_systemMask.set(rigidbodyType);
	}

	GridAABB* GridBroadPhaseSystem::m_GetAABB(uint idxAABB)
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		pGrid->LockRead();
		itIdxAABB itAABB = pGrid->mapAABBs.find(idxAABB);
		if (itAABB == pGrid->mapAABBs.end())
		{
			pGrid->UnlockRead();
			return nullptr;
		}

		pGrid->UnlockRead();
		return itAABB->second;
	}

	GridAABB* GridBroadPhaseSystem::m_GetAABB(const glm::vec3& point)
	{
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		pGrid->LockRead();
		uint idxAABB = GridUtils::LocatePoint(point, pGrid->lengthPerBox);
		pGrid->UnlockRead();

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
		pGrid->LockWrite();
		pAABB->minXYZ = GridUtils::LocatePosition(idxAABB, pGrid->lengthPerBox);
		pAABB->maxXYZ = pAABB->minXYZ + pGrid->lengthPerBox;

		pGrid->mapAABBs[idxAABB] = pAABB;
		pGrid->LockRead();

		return pAABB;
	}

	void GridBroadPhaseSystem::m_UpdateRigidBodies(std::shared_ptr<Scene> pScene, bool updatePassive)
	{
		// Creating AABBs grid
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		// Rigidbody entities
		for (Entity entityId : m_vecEntities)
		{
			TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
			RigidBodyComponent& rigidBody = pScene->Get<RigidBodyComponent>(entityId);

			rigidBody.LockRead();
			if (!updatePassive && rigidBody.bodyType == eBody::STATIC)
			{
				continue;
			}

			pGrid->LockRead();
			transform.LockRead();
			uint idxpos = GridUtils::LocatePoint(transform.worldPosition, pGrid->lengthPerBox);
			pGrid->UnlockRead();

			m_InsertEntity(entityId, idxpos, rigidBody.bodyType);

			m_InsertSphere(entityId, idxpos, transform.position,
						   rigidBody.radius, rigidBody.bodyType, pGrid);
			transform.UnlockRead();
			rigidBody.UnlockRead();
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

					pGrid->LockRead();
					uint currIdxpos = GridUtils::LocatePoint(currPoint, pGrid->lengthPerBox);
					pGrid->UnlockRead();

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

		pGrid->LockWrite();
		for (pairIdxAABB pairAABB : pGrid->mapAABBs)
		{
			delete pairAABB.second;
		}

		pGrid->mapAABBs.clear();
		pGrid->UnlockWrite();
	}

	size_t GridBroadPhaseSystem::m_RemoveAABB(uint idxAABB)
	{
		GridAABB* pAABB = m_GetAABB(idxAABB);
		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();

		if (!pAABB)
		{
			return 0;
		}

		pGrid->LockWrite();
		size_t left = pGrid->mapAABBs.erase(idxAABB);
		delete pAABB;
		pGrid->UnlockWrite();

		return left;
	}
}
