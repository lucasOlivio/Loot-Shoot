#include "pch.h"

#include "ParticleEmissionSystem.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Graphics/Components/Components.h"
#include "Engine/Graphics/Particles/ParticleManagerLocator.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/Random.h"

namespace MyEngine
{
    void ParticleEmissionSystem::Init()
    {
    }

    void ParticleEmissionSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);

        // Load all particles models
        std::shared_ptr<TextureManager> pTextureManager = ResourceManagerFactory::GetOrCreate<TextureManager>(eResourceTypes::TEXTURE);
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            emitter.LockWrite();
            emitter.properties.textureIndex = pTextureManager->LoadResource(emitter.properties.texture);
            emitter.UnlockWrite();
        }

        // Kill all particles
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();

        pParticleManager->ResetParticles();

        // Reset all emitters count
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            emitter.LockWrite();
            emitter.totalEmitPart = 0;
            emitter.UnlockWrite();
        }
    }

    void ParticleEmissionSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();
        std::shared_ptr<TimerComponent> pTimer = CoreLocator::GetTimer();

        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);
            
            if (!emitter.isActive)
            {
                continue;
            }

            emitter.LockWrite();
            emitter.timer += deltaTime;
            emitter.timeLastEmit += deltaTime;
            emitter.UnlockWrite();

            // Get generate one particle every x time
            uint32_t seed = static_cast<uint32_t>(pTimer->miliseconds + entityId);

            emitter.LockRead();
            int randomRate = Random::Int(seed, emitter.emitRateMin, emitter.emitRateMax);
            float oneParticleTimer = 1.0f / randomRate;
            if (emitter.timeLastEmit < oneParticleTimer)
            {
                // Not time to create one particle yet
                emitter.UnlockRead();
                continue;
            }

            int particlesToCreate = static_cast<int>(emitter.timeLastEmit * randomRate);

            if (emitter.totalEmitPart > emitter.maxParticles)
            {
                emitter.UnlockRead();
                continue;
            }

            if (emitter.totalEmitPart + particlesToCreate > emitter.maxParticles)
            {
                particlesToCreate = emitter.maxParticles - emitter.totalEmitPart;
            }

            // Emit particle with random properties
            EmitterProps& emitterProps = emitter.properties;

            transform.LockRead();
            glm::vec3 position = transform.position;
            transform.UnlockRead();

            for (int i = 0; i < particlesToCreate; i++)
            {
                ParticleProps particle;

                particle.entityId = entityId;

                particle.lifetime = Random::Float(seed, emitterProps.minLifeTime, emitterProps.maxLifeTime);

                particle.acceleration = emitterProps.constForce;
                particle.velocity = Random::Vec3(seed, emitterProps.velMin, emitterProps.velMax);
                particle.rotationSpeed = Random::Vec3(seed, emitterProps.rotMin, emitterProps.rotMax);
                
                particle.orientation = Random::Quat(seed, emitterProps.oriMin, emitterProps.oriMax);
                particle.position = position + Random::Vec3(seed, emitterProps.posMin, emitterProps.posMax);
                particle.scale = Random::Float(seed, emitterProps.scaMin, emitterProps.scaMax);
               
                particle.textureIndex = emitterProps.textureIndex;

                pParticleManager->EmitParticle(particle);
            }
            emitter.UnlockRead();

            emitter.LockWrite();
            emitter.totalEmitPart += particlesToCreate;
            emitter.timeLastEmit = 0.0f;
            emitter.UnlockWrite();
        }
    }

    void ParticleEmissionSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleEmissionSystem::End(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::End(pScene);

        // Kill all particles
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();

        pParticleManager->ResetParticles();
    }

    void ParticleEmissionSystem::Shutdown()
    {
    }

    void ParticleEmissionSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType emitterType = pScene->GetComponentType<EmitterComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(emitterType);
    }
}
