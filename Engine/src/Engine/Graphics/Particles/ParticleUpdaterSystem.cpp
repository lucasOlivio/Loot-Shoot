#include "pch.h"

#include "ParticleUpdaterSystem.h"

#include "Engine/Core/Threads/ThreadPoolLocator.h"

#include "Engine/Graphics/Particles/ParticleManagerLocator.h"
#include "Engine/Graphics/Components/Components.h"
#include "Engine/Graphics/Renderer/RendererManagerLocator.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    const size_t STEPS = 1000;

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
        std::shared_ptr<iThreadPool> pSysThreadPool = ThreadPoolLocator::GetOrCreate(SystemName());

        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();
        std::vector<ParticleProps>& particles = pParticleManager->GetParticles();

        ParticleProps* vecParticles = &particles[0];

        pParticleManager->LockWrite();
        for (size_t i = 0; i < MAX_PARTICLES;)
        {
            size_t endIndex = i + STEPS;
            if (endIndex > MAX_PARTICLES)
            {
                endIndex = MAX_PARTICLES;
            }

            pSysThreadPool->EnqueueTask([vecParticles, i, endIndex, pScene, deltaTime]()
            {
                ParticleUpdaterSystem::UpdateParticles(vecParticles, i, endIndex, pScene, deltaTime);
            });

            i = endIndex;
        }

        while (pSysThreadPool->RunningTasks() > 0)
        {
            Sleep(0);
        }
        pParticleManager->UnlockWrite();
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

    void ParticleUpdaterSystem::UpdateParticles(ParticleProps* vecParticles, 
                                                size_t startIndex, size_t endIndex,
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
            particle.position = particle.position + (particle.velocity * deltaTime);

            particle.orientation = TransformUtils::AdjustOrientation(particle.orientation, glm::vec3(particle.rotationSpeed * deltaTime));

            // Color change over time
            particle.color = particle.color + (particle.colorChange * deltaTime);
            // Particle total alpha from 1 to 0 to die
            particle.alpha = particle.lifetime / particle.initialLifeTime;
            particle.UnlockWrite();
        }
    }
}
