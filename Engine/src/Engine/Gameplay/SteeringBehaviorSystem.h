#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	class SteeringBehaviorSystem : public EntitySystem
	{
	public:
		SteeringBehaviorSystem() = default;
		virtual ~SteeringBehaviorSystem() { };

		virtual std::string SystemName() { return "SteeringBehaviorSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);

	private:
		void m_SeekTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
							glm::quat& myOrientation, glm::vec3& myVelocity,
							float speed, float maxDistance, float futureTime);
		
		void m_FleeFromTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
							  glm::quat& myOrientation, glm::vec3& myVelocity,
							  float speed, float maxDistance, float futureTime);
		
		void m_PursueTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
							glm::quat& myOrientation, glm::vec3& myVelocity, const glm::vec3& targetVelocity,
							float speed, float maxDistance, float futureTime);
		
		void m_EvadeTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
							glm::quat& myOrientation, glm::vec3& myVelocity, const glm::vec3& targetVelocity,
							float speed, float maxDistance, float futureTime);
		
		void m_ApproachTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
							  glm::quat& myOrientation, glm::vec3& myVelocity,
							  float speed, float maxDistance, float futureTime);
	};
}
