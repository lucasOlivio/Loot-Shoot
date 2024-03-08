#include "pch.h"

#include "MovementSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void MovementSystem::Init()
    {
    }

    void MovementSystem::Start(std::shared_ptr<Scene> pScene)
    {
        return;
    }

    void MovementSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : SceneView<TransformComponent, MovementComponent>(*pScene))
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            MovementComponent& movement = pScene->Get<MovementComponent>(entityId);

            glm::vec3 newVelocity = movement.velocity + (movement.acceleration * deltaTime);
            glm::vec3 dragForce = newVelocity * -(movement.drag * deltaTime);
            movement.velocity = newVelocity + dragForce;

            // Clip velocity between min and max
            if (movement.velocity.length() <= 0.5f || movement.maxSpeed == 0.0f)
            {
                movement.velocity = glm::vec3(0.0f);
            }
            else if (movement.velocity.length() > movement.maxSpeed)
            {
                movement.velocity = glm::normalize(movement.velocity) * movement.maxSpeed;
            }

            transform.position = transform.position + (movement.velocity * deltaTime);
        }
    }

    void MovementSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void MovementSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void MovementSystem::Shutdown()
    {
    }
}
