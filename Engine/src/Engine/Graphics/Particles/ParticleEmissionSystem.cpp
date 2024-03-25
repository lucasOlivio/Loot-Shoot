#include "pch.h"

#include "ParticleEmissionSystem.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/Textures/Texture.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Graphics/Components/Components.h"
#include "Engine/Graphics/Particles/ParticleManagerLocator.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/Random.h"
#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    void ParticleEmissionSystem::Init()
    {
    }

    void ParticleEmissionSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);

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

        std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
        std::shared_ptr<TextureManager> pTextureManager = ResourceManagerFactory::GetOrCreate<TextureManager>(eResourceTypes::TEXTURE);
        pShaderManager->ActivateResource(INSTANCING_SHADER);
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            // Load and create all textures
            for (const std::string& nameTexture : emitter.properties.textures)
            {
                size_t index = pTextureManager->LoadResource(nameTexture);
                pTextureManager->Create2DTexture(nameTexture, eTextureType::COLOR);
                std::shared_ptr<sTextureInfo> pTexture = std::static_pointer_cast<sTextureInfo>(pTextureManager->GetResource(index));

                emitter.properties.numTextures.push_back(pTexture->textNumber);
            }

        }
        pShaderManager->ActivateResource(DEFAULT_SHADER);
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
                ParticleProps& particle = pParticleManager->EmitParticle();

                particle.LockWrite();
                particle.entityId = entityId;

                particle.initialLifeTime = Random::Float(seed, emitterProps.minLifeTime, emitterProps.maxLifeTime);
                particle.lifetime = particle.initialLifeTime;

                particle.acceleration = emitterProps.constForce;
                particle.velocity = Random::Vec3(seed, emitterProps.velMin, emitterProps.velMax);
                
                particle.transform = glm::mat4(1.0f);
                TransformUtils::GetTransform(position + Random::Vec3(seed, emitterProps.posMin, emitterProps.posMax),
                                             Random::Float(seed, emitterProps.scaMin, emitterProps.scaMax),
                                             particle.transform);
               
                int indexTexture = Random::Int(seed, 0, emitter.properties.numTextures.size());
                particle.numTexture = emitter.properties.numTextures[indexTexture];

                particle.UnlockWrite();
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
