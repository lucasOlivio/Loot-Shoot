#include "pch.h"

#include "ParticleUpdaterSystem.h"

#include "Engine/Graphics/Particles/ParticleManagerLocator.h"
#include "Engine/Graphics/Components/Components.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void ParticleUpdaterSystem::Init()
    {
    }

    void ParticleUpdaterSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleUpdaterSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();

        const std::vector<ParticleProps>& particles = pParticleManager->GetParticles();

        for (int i = 0; i < particles.size(); i++)
        {
            ParticleProps particle = particles[i];

            if (particle.lifetime <= 0.0f)
            {
                continue;
            }

            particle.lifetime -= deltaTime;

            if (particle.lifetime <= 0.0f)
            {
                EmitterComponent& emitter = pScene->Get<EmitterComponent>(particle.entityId);
                emitter.totalEmitPart--;
            }

            // Euler integration
            particle.velocity = particle.velocity + (particle.acceleration * deltaTime);
            particle.position = particle.position + (particle.velocity * deltaTime);

            particle.orientation = TransformUtils::AdjustOrientation(particle.orientation, glm::vec3(particle.rotationSpeed * deltaTime));

            // Color change over time
            particle.color = particle.color + (particle.colorChange * deltaTime);
            // Particle total alpha from 1 to 0 to die
            particle.alpha = particle.lifetime / particle.initialLifeTime;

            pParticleManager->UpdateParticle(i, particle);
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
}
