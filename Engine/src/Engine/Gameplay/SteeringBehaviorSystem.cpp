#include "pch.h"

#include "SteeringBehaviorSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Gameplay/GameplayProperties.h"
#include "Engine/Gameplay/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/Math.h"
#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/GameplayUtils.h"

#include "Engine/Physics/Components/Components.h"

namespace MyEngine
{
    void SteeringBehaviorSystem::Init()
    {
    }

    void SteeringBehaviorSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void SteeringBehaviorSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : SceneView<TransformComponent, MovementComponent, SteeringBehaviorComponent>(*pScene))
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            MovementComponent& movement = pScene->Get<MovementComponent>(entityId);
            SteeringBehaviorComponent& steeringBehavior = pScene->Get<SteeringBehaviorComponent>(entityId);

            TransformComponent& transformTarget = pScene->Get<TransformComponent>(steeringBehavior.targetId);

            switch (steeringBehavior.steeringType)
            {
            case eSteeringTypes::SEEK:
                m_SeekTarget(transform.worldPosition, transformTarget.worldPosition,
							    transform.orientation, movement.velocity,
                                steeringBehavior.speed, steeringBehavior.maxDistance, 
                                steeringBehavior.futureTime);
                break;
            case eSteeringTypes::FLEE:
                m_FleeFromTarget(transform.worldPosition, transformTarget.worldPosition,
							    transform.orientation, movement.velocity,
                                steeringBehavior.speed, steeringBehavior.maxDistance, 
                                steeringBehavior.futureTime);
                break;
            case eSteeringTypes::PURSUE:
            {
                glm::vec3 targetVelocity = glm::vec3(0.0f);
                MovementComponent& movementTarget = pScene->Get<MovementComponent>(steeringBehavior.targetId);

                m_PursueTarget(transform.worldPosition, transformTarget.worldPosition,
                    transform.orientation, movement.velocity, movementTarget.velocity,
                    steeringBehavior.speed, steeringBehavior.maxDistance,
                    steeringBehavior.futureTime);
            }
                break;
            case eSteeringTypes::EVADE:
            {
                glm::vec3 targetVelocity = glm::vec3(0.0f);
                MovementComponent& movementTarget = pScene->Get<MovementComponent>(steeringBehavior.targetId);

                m_EvadeTarget(transform.worldPosition, transformTarget.worldPosition,
                    transform.orientation, movement.velocity, movementTarget.velocity,
                    steeringBehavior.speed, steeringBehavior.maxDistance,
                    steeringBehavior.futureTime);
            }
            break;
            case eSteeringTypes::APPROACH:
                m_ApproachTarget(transform.worldPosition, transformTarget.worldPosition,
                    transform.orientation, movement.velocity,
                    steeringBehavior.speed, steeringBehavior.maxDistance,
                    steeringBehavior.futureTime);
            break;
            default:
                break;
            }
        }
    }

    void SteeringBehaviorSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void SteeringBehaviorSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void SteeringBehaviorSystem::Shutdown()
    {
    }

    void SteeringBehaviorSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType movementType = pScene->GetComponentType<MovementComponent>();
        ComponentType behaviorType = pScene->GetComponentType<SteeringBehaviorComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(movementType);
        m_systemMask.set(behaviorType);
    }

    void SteeringBehaviorSystem::m_SeekTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
        glm::quat& myOrientation, glm::vec3& myVelocity,
        float speed, float maxDistance, float futureTime)
    {
        GameplayUtils::CalculateSteeringDirections(myPosition, targetPosition, myOrientation, myVelocity,
            glm::vec3(0.0f), false, speed, maxDistance, futureTime,
            [](float distToTarget, float maxDistance) {
                return distToTarget < maxDistance;
            });
    }

    void SteeringBehaviorSystem::m_FleeFromTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
        glm::quat& myOrientation, glm::vec3& myVelocity,
        float speed, float maxDistance, float futureTime)
    {
        GameplayUtils::CalculateSteeringDirections(myPosition, targetPosition, myOrientation, myVelocity,
            glm::vec3(0.0f), true, speed, maxDistance, futureTime,
            [](float distToTarget, float maxDistance) {
                return distToTarget > maxDistance;
            });
    }

    void SteeringBehaviorSystem::m_PursueTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
        glm::quat& myOrientation, glm::vec3& myVelocity,
        const glm::vec3& targetVelocity, float speed, float maxDistance, float futureTime)
    {
        GameplayUtils::CalculateSteeringDirections(myPosition, targetPosition, myOrientation, myVelocity,
            targetVelocity, false, speed, maxDistance, futureTime,
            [](float distToTarget, float maxDistance) {
                return distToTarget < maxDistance;
            });
    }

    void SteeringBehaviorSystem::m_EvadeTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
        glm::quat& myOrientation, glm::vec3& myVelocity,
        const glm::vec3& targetVelocity, float speed, float maxDistance, float futureTime)
    {
        GameplayUtils::CalculateSteeringDirections(myPosition, targetPosition, myOrientation, myVelocity,
            targetVelocity, true, speed, maxDistance, futureTime,
            [](float distToTarget, float maxDistance) {
                return distToTarget > maxDistance;
            });
    }

    void SteeringBehaviorSystem::m_ApproachTarget(const glm::vec3& myPosition, const glm::vec3& targetPosition,
        glm::quat& myOrientation, glm::vec3& myVelocity,
        float speed, float maxDistance, float futureTime)
    {
        GameplayUtils::CalculateSteeringDirections(myPosition, targetPosition, myOrientation, myVelocity,
                        glm::vec3(0.0f), false, speed, maxDistance, futureTime,
                        [](float distToTarget, float maxDistance) {
                            float distToMaxDist = maxDistance - distToTarget;
                            return (distToMaxDist < 1.0f && distToMaxDist > -1.0f);
                        });
    }
}
