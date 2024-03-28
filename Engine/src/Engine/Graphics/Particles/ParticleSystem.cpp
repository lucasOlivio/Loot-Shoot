#include "pch.h"

#include "ParticleSystem.h"

#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Resources/Meshes/Mesh.h"
#include "Engine/Core/Resources/Meshes/MeshManager.h"
#include "Engine/Core/Resources/Textures/Texture.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"

#include "Engine/Utils/CameraUtils.h"

namespace MyEngine
{
    const std::string QUAD_MESH = "flat.ply";

    void ParticleSystem::Init()
    {
    }

    void ParticleSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);

        // Reset all emitters count
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            emitter.LockWrite();
            emitter.totalEmitPart = 0;
            emitter.particles.resize(emitter.maxParticles);
            emitter.freeList.resize(emitter.maxParticles + 1, 0);
            emitter.freeList[0] = emitter.maxParticles;
            emitter.UnlockWrite();
        }

        std::shared_ptr<MeshManager> pMeshManager = ResourceManagerFactory::GetOrCreate<MeshManager>(eResourceTypes::MESH);
        std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
        std::shared_ptr<TextureManager> pTextureManager = ResourceManagerFactory::GetOrCreate<TextureManager>(eResourceTypes::TEXTURE);
        
        pShaderManager->LoadComputeShader(SPAWN_PARTICLES_SHADER);
        pShaderManager->LoadComputeShader(UPDATE_PARTICLES_SHADER);

        pShaderManager->ActivateResource(PARTICLES_SHADER);
        for (Entity entityId : m_vecEntities)
        {
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            // Load and create all textures
            size_t index = pTextureManager->LoadResource(emitter.nameTexture);
            pTextureManager->Create2DTexture(emitter.nameTexture, eTextureType::COLOR);
            std::shared_ptr<sTextureInfo> pTexture = std::static_pointer_cast<sTextureInfo>(pTextureManager->GetResource(index));

            emitter.numTexture = pTexture->textNumber;

            // Load quad mash VBO
            emitter.pMesh = pMeshManager->LoadParticles(QUAD_MESH, emitter.particles, emitter.freeList, 
                                                        emitter.ssbo0, emitter.ssbo1);
        }

        pShaderManager->ActivateResource(DEFAULT_SHADER);
    }

    void ParticleSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void ParticleSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        m_ParticlesUpdate(pScene, deltaTime);
        m_ParticlesRender(pScene);
    }

    void ParticleSystem::End(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::End(pScene);
    }

    void ParticleSystem::Shutdown()
    {
    }

    void ParticleSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType emitterType = pScene->GetComponentType<EmitterComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(emitterType);
    }

    void ParticleSystem::m_SetEmitterUniforms(TransformComponent& transform, 
								              EmitterComponent& emitter, 
								              std::shared_ptr<ShaderManager> pShaderManager,
                                              uint particlesToSpawn)
    {
        // Set the uniform variables in the shader struct EmitterSettings
        pShaderManager->SetUniformInt("particlesToSpawn", particlesToSpawn);
        pShaderManager->SetUniformVec3("emitter.minOffset", emitter.posMin);
        pShaderManager->SetUniformVec3("emitter.maxOffset", emitter.posMax);
        pShaderManager->SetUniformVec3("emitter.minVelocity", emitter.velMin);
        pShaderManager->SetUniformVec3("emitter.maxVelocity", emitter.velMax);
        pShaderManager->SetUniformVec3("emitter.minAcceleration", emitter.accMin);
        pShaderManager->SetUniformVec3("emitter.maxAcceleration", emitter.accMax);
        pShaderManager->SetUniformFloat("emitter.minScale", emitter.scaMin);
        pShaderManager->SetUniformFloat("emitter.maxScale", emitter.scaMax);
        pShaderManager->SetUniformFloat("emitter.minLife", emitter.minLifeTime);
        pShaderManager->SetUniformFloat("emitter.maxLife", emitter.maxLifeTime);
        pShaderManager->SetUniformVec3("emitter.position", transform.position);
    }

    void ParticleSystem::m_ParticlesUpdate(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<TimerComponent> pTimer = CoreLocator::GetTimer();
        std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        // ensure that writes from last frame's updates are complete
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // particle spawn loop- one dispatch per emitter
        pShaderManager->ActivateResource(SPAWN_PARTICLES_SHADER);
        pShaderManager->SetUniformInt("currMS", pTimer->miliseconds);
        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            emitter.timer += deltaTime;

            uint particlesToSpawn = emitter.timer / emitter.spawnInterval;
            emitter.timer = fmod(emitter.timer, emitter.spawnInterval);

            if (particlesToSpawn <= 0)
                continue;

            m_SetEmitterUniforms(transform, emitter, pShaderManager, particlesToSpawn);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter.ssbo0);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter.ssbo1);

            // run the compute shader
            int workGroupSize = 64;
            int numWorkGroups = (particlesToSpawn + workGroupSize - 1) / workGroupSize;
            glDispatchCompute(numWorkGroups, 1, 1);
        }

        // ensure that the spawned particles are visible in particle updates
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

        // particle update loop- again, one dispatch per emitter
        pShaderManager->ActivateResource(UPDATE_PARTICLES_SHADER);
        pShaderManager->SetUniformFloat("deltaTime", deltaTime);
        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter.ssbo0);
            glBindBuffer(GL_SHADER_STORAGE_BUFFER, emitter.ssbo1);

            // run the compute shader
            int workGroupSize = 128; // or the constant 128
            int numWorkGroups = (emitter.maxParticles + workGroupSize - 1) / workGroupSize;
            glDispatchCompute(numWorkGroups, 1, 1);
        }
    }

    void ParticleSystem::m_ParticlesRender(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        pShaderManager->ActivateResource(PARTICLES_SHADER);
        CameraUtils::UpdateCamera(pScene);

        glActiveTexture(GL_TEXTURE0);

        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        pShaderManager->SetUniformInt("textureColor", GL_TEXTURE0);

        // render each emitter with one draw call
        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            EmitterComponent& emitter = pScene->Get<EmitterComponent>(entityId);

            // Bind the VBO to update its data
            glBindVertexArray(emitter.pMesh->VAO_ID);
            glBindBuffer(GL_ARRAY_BUFFER, emitter.ssbo0);
            glBindTexture(GL_TEXTURE_2D, emitter.numTexture);

            pShaderManager->SetUniformInt("rowsTexture", emitter.rowsTexture);
            pShaderManager->SetUniformInt("colsTexture", emitter.colsTexture);

            glDrawElementsInstanced(
                GL_TRIANGLES, emitter.pMesh->numberOfIndices, GL_UNSIGNED_INT, 0, emitter.maxParticles
            );

            // Unbind the VBO
            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        glDepthMask(GL_TRUE);
    }
}
