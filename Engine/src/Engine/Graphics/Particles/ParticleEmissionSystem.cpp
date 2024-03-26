#include "pch.h"

#include "ParticleEmissionSystem.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/Meshes/Mesh.h"
#include "Engine/Core/Resources/Meshes/MeshManager.h"
#include "Engine/Core/Resources/Textures/Texture.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Graphics/Renderer/RendererManagerLocator.h"
#include "Engine/Graphics/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/Random.h"
#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
    const std::string QUAD_MESH = "flat.ply";
    std::shared_ptr<sMeshInfo> meshInfo = nullptr;
    uint particlesVBO = 0;

    void ParticleEmissionSystem::Init()
    {
    }

    void ParticleEmissionSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);

        // Reset all emitters count
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            emitter.LockWrite();
            emitter.totalEmitPart = 0;
            emitter.particles.resize(emitter.maxParticles);
            emitter.UnlockWrite();
        }

        std::shared_ptr<MeshManager> pMeshManager = ResourceManagerFactory::GetOrCreate<MeshManager>(eResourceTypes::MESH);
        std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
        std::shared_ptr<TextureManager> pTextureManager = ResourceManagerFactory::GetOrCreate<TextureManager>(eResourceTypes::TEXTURE);
        pShaderManager->ActivateResource(INSTANCING_SHADER);
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            // Load and create all textures
            size_t index = pTextureManager->LoadResource(emitter.nameTexture);
            pTextureManager->Create2DTexture(emitter.nameTexture, eTextureType::COLOR);
            std::shared_ptr<sTextureInfo> pTexture = std::static_pointer_cast<sTextureInfo>(pTextureManager->GetResource(index));

            emitter.numTexture = pTexture->textNumber;

            // Load quad mash VBO
            emitter.pMesh = pMeshManager->LoadParticles(QUAD_MESH, emitter.particles, emitter.vboId);
        }

        pShaderManager->ActivateResource(DEFAULT_SHADER);
    }

    void ParticleEmissionSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
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

            int randomRate = Random::Int(seed, emitter.emitRateMin, emitter.emitRateMax);
            float oneParticleTimer = 1.0f / randomRate;
            if (emitter.timeLastEmit < oneParticleTimer)
            {
                // Not time to create one particle yet
                continue;
            }

            int particlesToCreate = static_cast<int>(emitter.timeLastEmit * randomRate);

            if (emitter.totalEmitPart >= emitter.maxParticles)
            {
                continue;
            }

            if (emitter.totalEmitPart + particlesToCreate >= emitter.maxParticles)
            {
                particlesToCreate = emitter.maxParticles - emitter.totalEmitPart;
            }

            transform.LockRead();
            glm::vec3 position = transform.position;
            transform.UnlockRead();

            for (int i = 0; i < particlesToCreate; i++)
            {
                ParticleProps& particle = emitter.particles[emitter.nextParticle];

                particle.LockWrite();
                particle.lifetime = Random::Float(seed, emitter.minLifeTime, emitter.maxLifeTime);

                particle.acceleration = Random::Vec3(seed, emitter.accMin, emitter.accMax);
                particle.velocity = Random::Vec3(seed, emitter.velMin, emitter.velMax);
                
                particle.transform = glm::mat4(1.0f);
                TransformUtils::GetTransform(position + Random::Vec3(seed, emitter.posMin, emitter.posMax),
                                             Random::Float(seed, emitter.scaMin, emitter.scaMax),
                                             particle.transform);
                particle.UnlockWrite();

                InterlockedIncrement(&emitter.totalEmitPart);
                emitter.nextParticle = (emitter.nextParticle + 1) % emitter.maxParticles;
            }

            emitter.LockWrite();
            emitter.timeLastEmit = 0.0f;
            emitter.UnlockWrite();
        }
    }

    void ParticleEmissionSystem::Render(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<iRendererManager> pRenderer = RendererManagerLocator::Get();
        std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        pShaderManager->ActivateResource(INSTANCING_SHADER);
        pRenderer->UpdateCamera(pScene);

        glActiveTexture(GL_TEXTURE0);

        pShaderManager->SetUniformInt("textureColor", GL_TEXTURE0);

        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            // Bind the VBO to update its data
            glBindVertexArray(emitter.pMesh->VAO_ID);
            glBindBuffer(GL_ARRAY_BUFFER, emitter.vboId);
            glBindTexture(GL_TEXTURE_2D, emitter.numTexture);

            // Update the VBO with the new data
            emitter.LockRead();
            size_t numParticles = emitter.maxParticles;
            glBufferSubData(GL_ARRAY_BUFFER, 0,
                (sizeof(ParticleProps) * numParticles),
                &(emitter.particles[0]));
            emitter.UnlockRead();

            glDrawElementsInstanced(
                GL_TRIANGLES, emitter.pMesh->numberOfIndices, GL_UNSIGNED_INT, 0, numParticles
            );

            // Unbind the VBO
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        pShaderManager->ActivateResource(DEFAULT_SHADER);
    }

    void ParticleEmissionSystem::End(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::End(pScene);
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
