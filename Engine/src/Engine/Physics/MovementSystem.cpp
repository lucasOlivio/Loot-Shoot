#include "pch.h"

#include "MovementSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Physics/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/Random.h"

namespace MyEngine
{
    void MovementSystem::Init()
    {
    }

    void MovementSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);
    }

    void MovementSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            MovementComponent& movement = pScene->Get<MovementComponent>(entityId);

            movement.LockWrite();
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

            transform.LockWrite();
            transform.position = transform.position + (movement.velocity * deltaTime);

            if (transform.position.y <= 0.0f)
            {
                transform.position.y = 0.0f;
            }
            transform.UnlockWrite();
            movement.UnlockWrite();
        }
    }

    void MovementSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void MovementSystem::End(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::End(pScene);
    }

    void MovementSystem::Shutdown()
    {
    }

    void MovementSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType movementType = pScene->GetComponentType<MovementComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(movementType);
    }
}
