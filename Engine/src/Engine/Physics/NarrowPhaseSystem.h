#pragma once

#include "Engine/Core/Components/Components.h"

#include "Engine/Physics/Components/Components.h"

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	class NarrowPhaseSystem : public iSystem
	{
	public:
		NarrowPhaseSystem() = default;
		virtual ~NarrowPhaseSystem() { };

		virtual std::string SystemName() { return "NarrowPhaseSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	private:
		// Returns false if the collision is duplicated
		bool m_RegisterFrameCollision(const sCollisionData& collData);

		// Use the event bus to trigger a collision event with this event data
		void m_TriggerCollision(const sCollisionData& collData);

		// Sphere checks
		void m_CheckSphereOverlaps(std::shared_ptr<Scene> pScene,
								   Entity entityIdA,
								   TransformComponent& transformA,
								   RigidBodyComponent& sphereA,
								   const int index,
								   const std::vector<Entity>& activeEntities,
								   const std::vector<Entity>& passiveEntities,
								   const std::vector<Entity>& staticEntities);

		bool m_CheckSphereEntityOverlap(std::shared_ptr<Scene> pScene,
										Entity entityIdA,
										TransformComponent& transformA,
										RigidBodyComponent& sphereA,
										Entity entityIdB);
	};
}
