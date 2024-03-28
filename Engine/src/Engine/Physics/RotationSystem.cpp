#include "pch.h"

#include "RotationSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Physics/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/Random.h"

namespace MyEngine
{
    void RotationSystem::Init()
    {
    }

    void RotationSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);
    }

    void RotationSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Update velocity and position
        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            RotationComponent& rotation = pScene->Get<RotationComponent>(entityId);

            rotation.LockWrite();
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
            rotation.UnlockWrite();

            transform.LockWrite();
            transform.orientation = TransformUtils::AdjustOrientation(transform.orientation, deltaRotation);
            transform.UnlockWrite();
        }
    }

    void RotationSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void RotationSystem::End(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::End(pScene);
    }

    void RotationSystem::Shutdown()
    {
    }

    void RotationSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType rotationType = pScene->GetComponentType<RotationComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(rotationType);
    }
}
