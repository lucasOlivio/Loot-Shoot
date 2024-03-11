#include "pch.h"

#include "ParticleEmissionSystem.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"

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
        // Load all particles models
        std::shared_ptr<iResourceManager> pMeshManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MESH);
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            size_t index = pMeshManager->LoadResource(emitter.properties.meshName);

            emitter.pMesh = std::static_pointer_cast<sMeshInfo>(pMeshManager->GetResource(index));
        }

        // Kill all particles
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();

        pParticleManager->ResetParticles();

        // Reset all emitters count
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            emitter.totalEmitPart = 0;
        }
    }

    void ParticleEmissionSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<iParticleManager> pParticleManager = ParticleManagerLocator::Get();

        float timeScale = (deltaTime / 60.0f);

        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            if (!emitter.isActive)
            {
                continue;
            }

            emitter.timer += deltaTime;
            emitter.timeLastEmit += deltaTime;

            // Get generate one particle every x time
            uint32_t seed = (uint32_t)(entityId + emitter.totalEmitPart + (deltaTime * 1000));
            int randomRate = Random::Int(seed, emitter.emitRateMin, emitter.emitRateMax);
            float oneParticleTimer = 1.0f / randomRate;
            if (emitter.timeLastEmit < oneParticleTimer)
            {
                // Not time to create one particle yet
                continue;
            }

            int particlesToCreate = static_cast<int>(emitter.timeLastEmit * randomRate);

            if (emitter.totalEmitPart > emitter.maxParticles)
            {
                continue;
            }

            if (emitter.totalEmitPart + particlesToCreate > emitter.maxParticles)
            {
                particlesToCreate = emitter.maxParticles - emitter.totalEmitPart;
            }

            // Emit particle with random properties
            EmitterProps emitterProps = emitter.properties;
            for (int i = 0; i < particlesToCreate; i++)
            {
                ParticleProps particle;

                particle.entityId = entityId;

                particle.FBOIDs = emitterProps.FBOIDs;

                particle.lifetime = Random::Float(seed, emitterProps.minLifeTime, emitterProps.maxLifeTime);

                particle.acceleration = emitterProps.constForce;
                particle.velocity = Random::Vec3(seed, emitterProps.velMin, emitterProps.velMax);
                particle.rotationSpeed = Random::Vec3(seed, emitterProps.rotMin, emitterProps.rotMax);
                
                particle.orientation = Random::Quat(seed, emitterProps.oriMin, emitterProps.oriMax);
                particle.position = transform.position + Random::Vec3(seed, emitterProps.posMin, emitterProps.posMax);
                particle.scale = Random::Float(seed, emitterProps.scaMin, emitterProps.scaMax);
               
                particle.pMesh = emitter.pMesh;
                particle.material = emitterProps.material;
                particle.defaultColor = emitterProps.colorInitial;
                particle.colorChange = emitterProps.colorChange;

                pParticleManager->EmitParticle(particle);
            }

            emitter.totalEmitPart += particlesToCreate;
            emitter.timeLastEmit = 0.0f;
        }
    }

    void ParticleEmissionSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void ParticleEmissionSystem::End(std::shared_ptr<Scene> pScene)
    {
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
