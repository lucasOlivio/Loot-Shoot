#include "pch.h"

#include "ParticleUpdaterSystem.h"

#include "Engine/Core/Threads/ThreadPoolLocator.h"
#include "Engine/Core/Components/Components.h"

#include "Engine/Graphics/Particles/ParticleManagerLocator.h"
#include "Engine/Graphics/Components/SingletonComponents.h"
#include "Engine/Graphics/Renderer/RendererManagerLocator.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/CameraUtils.h"

namespace MyEngine
{
    void ParticleUpdaterSystem::Init()
    {
    }

    void ParticleUpdaterSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);
    }

    void ParticleUpdaterSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        CameraComponent& camera = pScene->Get<CameraComponent>(CAMERA_ID);
        TransformComponent& transformCamera = pScene->Get<TransformComponent>(CAMERA_ID);
        glm::vec3 cameraPosition = CameraUtils::FinalPosition(transformCamera.position, transformCamera.orientation,
                                                                camera.distance, camera.height, camera.offsetTarget);

        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);
            ParticleProps* vecParticles = &emitter.particles[0];

            emitter.LockWrite();
            for (size_t i = 0; i < emitter.maxParticles; i++)
            {
                ParticleProps& particle = vecParticles[i];

                particle.LockWrite();
                if (particle.lifetime <= 0.0f)
                {
                    particle.UnlockWrite();
                    continue;
                }

                particle.lifetime -= deltaTime;

                if (particle.lifetime <= 0.0f)
                {
                    particle.UnlockWrite();
                    InterlockedDecrement(&emitter.totalEmitPart);
                    continue;
                }

                // Euler integration
                particle.velocity = particle.velocity + (particle.acceleration * deltaTime);
                particle.transform = glm::translate(particle.transform, (particle.velocity * deltaTime));
                particle.UnlockWrite();
            }
            emitter.UnlockWrite();
        }
    }

    void ParticleUpdaterSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleUpdaterSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleUpdaterSystem::Shutdown()
    {
    }

    void ParticleUpdaterSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType emitterType = pScene->GetComponentType<EmitterComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(emitterType);
    }
}
