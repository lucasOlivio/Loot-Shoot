#include "pch.h"

#include "RotationSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void RotationSystem::Init()
    {
    }

    void RotationSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void RotationSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : SceneView<TransformComponent, RotationComponent>(*pScene))
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            RotationComponent& rotation = pScene->Get<RotationComponent>(entityId);

            glm::vec3 newVelocity = rotation.velocity + (rotation.acceleration * deltaTime);
            glm::vec3 dragForce = newVelocity * -(rotation.drag * deltaTime);
            rotation.velocity = newVelocity + dragForce;

            // Clip velocity between min and max
            if (rotation.velocity.length() <= 0.5f || rotation.maxSpeed == 0.0f)
            {
                rotation.velocity = glm::vec3(0.0f);
            }
            else if (rotation.velocity.length() > rotation.maxSpeed)
            {
                rotation.velocity = glm::normalize(rotation.velocity) * rotation.maxSpeed;
            }

            glm::vec3 deltaRotation = rotation.velocity * deltaTime;

            transform.orientation = TransformUtils::AdjustOrientation(transform.orientation, deltaRotation);
        }
    }

    void RotationSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void RotationSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void RotationSystem::Shutdown()
    {
    }
}
