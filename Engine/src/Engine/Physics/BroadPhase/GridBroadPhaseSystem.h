#pragma once

#include "GridAABB.h"



#include "Engine/ECS/System/iSystem.h"

#include "Engine/Core/Shapes.hpp"

#include "Engine/Physics/PhysicsProperties.h"
#include "Engine/Physics/Components/SingletonComponents.h"

#include <map>

namespace MyEngine
{
	class GridBroadPhaseSystem : public iSystem
	{
	public:
		GridBroadPhaseSystem() = default;
		virtual ~GridBroadPhaseSystem() {};

		virtual std::string SystemName() { return "GridBroadPhaseSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	protected:
		GridAABB* m_GetAABB(uint idxAABB);
		GridAABB* m_GetAABB(const glm::vec3& point);

		GridAABB* m_GetOrCreateAABB(uint idxAABB);

		void m_UpdateRigidBodies(std::shared_ptr<Scene> pScene, bool updatePassive = false);

		// Calculates all the aabbs that the shape intersects
		// and inserts into the respective Grid AABB
		void m_InsertSphere(Entity entityID, uint originIndex,
							const glm::vec3& position, float radius, const eBody& bodyType,
							std::shared_ptr<GridBroadphaseComponent> pGrid);

		// Insert entity in map and active map if not a static body
		void m_InsertEntity(Entity entityID, uint index, const eBody& bodyType);

		// Remove AABB from mapping if exists
		size_t m_RemoveAABB(uint idxAABB);

		void m_ClearAABBs();
	};
}
