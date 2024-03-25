#include "pch.h"

#include "ParticleUpdaterSystem.h"

#include "Engine/Core/Threads/ThreadPoolLocator.h"
#include "Engine/Core/Components/Components.h"

#include "Engine/Graphics/Particles/ParticleManagerLocator.h"
#include "Engine/Graphics/Components/Components.h"
#include "Engine/Graphics/Components/SingletonComponents.h"
#include "Engine/Graphics/Renderer/RendererManagerLocator.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/CameraUtils.h"

namespace MyEngine
{
    const size_t STEPS = 1'000;

    void ParticleUpdaterSystem::Init()
    {
    }

    void ParticleUpdaterSystem::Start(std::shared_ptr<Scene> pScene)
    {
        // Initializes a pool for this system
        ThreadPoolLocator::GetOrCreate(SystemName());
    }

    void ParticleUpdaterSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();
        if (pParticleManager->ReadyToDraw())
        {
            return;
        }
            
        std::vector<ParticleProps>& particles = pParticleManager->GetParticles();
        std::shared_ptr<iThreadPool> pSysThreadPool = ThreadPoolLocator::GetOrCreate(SystemName());

        ParticleProps* vecParticles = &particles[0];

        CameraComponent& camera = pScene->Get<CameraComponent>(CAMERA_ID);
        TransformComponent& transformCamera = pScene->Get<TransformComponent>(CAMERA_ID);

        glm::vec3 cameraPosition = CameraUtils::FinalPosition(transformCamera.position, transformCamera.orientation,
                                                                camera.distance, camera.height, camera.offsetTarget);

        for (size_t i = 0; i < MAX_PARTICLES;)
        {
            size_t endIndex = i + STEPS;
            if (endIndex > MAX_PARTICLES)
            {
                endIndex = MAX_PARTICLES;
            }

            pSysThreadPool->EnqueueTask([pParticleManager, vecParticles, i, endIndex, cameraPosition, pScene, deltaTime]()
            {
                ParticleUpdaterSystem::UpdateParticles(pParticleManager, vecParticles, i, endIndex, 
                                                        cameraPosition, pScene, deltaTime);
            });

            i = endIndex;
        }

        while (pSysThreadPool->RunningTasks() > 0)
        {
            Sleep(0);
        }

        pParticleManager->SetReadyToDraw(true);
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

    void ParticleUpdaterSystem::UpdateParticles(std::shared_ptr<iParticleManager> pParticleManager,
                                                ParticleProps* vecParticles,
                                                size_t startIndex, size_t endIndex,
                                                glm::vec3 cameraPosition,
								                std::shared_ptr<Scene> pScene, float deltaTime)
    {
        for (size_t i = startIndex; i < endIndex; i++)
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
                EmitterComponent& emitter = pScene->Get<EmitterComponent>(particle.entityId);
                emitter.totalEmitPart--;
                particle.UnlockWrite();
                continue;
            }

            // Euler integration
            particle.velocity = particle.velocity + (particle.acceleration * deltaTime);
            particle.transform = glm::translate(particle.transform, (particle.velocity * deltaTime));

            // Particle total alpha from 1 to 0 to die
            particle.alpha = particle.lifetime / particle.initialLifeTime;
            particle.UnlockWrite();

            pParticleManager->SendToDraw(particle.numTexture, particle);
        }
    }
}
