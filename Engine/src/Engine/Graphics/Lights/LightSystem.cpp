#include "pch.h"

#include "LightSystem.h"

#include "Engine/Core/Resources/ResourceManagerFactory.h"

namespace MyEngine
{
    void LightSystem::Init()
    {
    }

    void LightSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);

        // Go over each lights setting the ULs and initializing them
        int lightIndex = 0;
        for (Entity entityId : m_vecEntities)
        {
            LightComponent& light = pScene->Get<LightComponent>(entityId);
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);

            m_SetupLight(transform, light, lightIndex);
            lightIndex++;
        }
    }

    void LightSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void LightSystem::Render(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        // Set lights to transform position of entity
        for (Entity entityId : m_vecEntities)
        {
            LightComponent& light = pScene->Get<LightComponent>(entityId);
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);

            m_UpdatePositionUL(transform, light, pShader);
            m_UpdateDirectionUL(light, pShader);
            m_UpdateDiffuseUL(light, pShader);
            m_UpdateSpecularUL(light, pShader);
            m_UpdateAttenUL(light, pShader);
            m_UpdateParamsUL(light, pShader);
            m_UpdateStatusUL(light, pShader);
        }
    }

    void LightSystem::End(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::End(pScene);

        // Reset all lights in shader with new empty components
        LightComponent light = LightComponent();
        TransformComponent transform = TransformComponent();
        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            m_SetupLight(transform, light, i);
        }
    }

    void LightSystem::Shutdown()
    {
    }

    void LightSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType lightType = pScene->GetComponentType<LightComponent>();
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();

        m_systemMask.set(lightType);
        m_systemMask.set(transformType);
    }

    void LightSystem::m_SetupLight(TransformComponent& transform,
                                   LightComponent& light, int lightIndex)
    {
        std::string ulBasePath = "theLights[" + std::to_string(lightIndex) + "].";
        std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        light.ulBasePath = ulBasePath;

        // Initial update in uniforms
        m_UpdatePositionUL(transform, light, pShader);
        m_UpdateDirectionUL(light, pShader);
        m_UpdateDiffuseUL(light, pShader);
        m_UpdateSpecularUL(light, pShader);
        m_UpdateAttenUL(light, pShader);
        m_UpdateParamsUL(light, pShader);
        m_UpdateStatusUL(light, pShader);
    }

    void LightSystem::m_UpdatePositionUL(TransformComponent& transform, 
                                         LightComponent& light, 
                                         std::shared_ptr<ShaderManager> pShader)
    {
        transform.LockRead();
        light.LockRead();
        glm::vec4 newPosition = glm::vec4(transform.position, 0) + light.positionOffset +
                               (light.direction * light.directionOffset);
        light.UnlockRead();
        transform.UnlockRead();

        pShader->SetUniformVec4((light.ulBasePath + "position").c_str(), newPosition);
    }

    void LightSystem::m_UpdateDirectionUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader)
    {
        light.LockRead();
        glm::vec4 newDirection = light.direction + light.directionOffset;
        pShader->SetUniformVec4((light.ulBasePath + "direction").c_str(), newDirection);
        light.UnlockRead();
    }

    void LightSystem::m_UpdateDiffuseUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader)
    {
        light.LockRead();
        pShader->SetUniformVec4((light.ulBasePath + "diffuse").c_str(), light.diffuse);
        light.UnlockRead();
    }

    void LightSystem::m_UpdateSpecularUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader)
    {
        light.LockRead();
        pShader->SetUniformVec4((light.ulBasePath + "specular").c_str(), light.specular);
        light.UnlockRead();
    }

    void LightSystem::m_UpdateAttenUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader)
    {
        light.LockRead();
        glm::vec4 newAtten = light.atten * light.flickerOffset;
        pShader->SetUniformVec4((light.ulBasePath + "atten").c_str(), newAtten);
        light.UnlockRead();
    }

    void LightSystem::m_UpdateParamsUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader)
    {
        light.LockRead();
        pShader->SetUniformVec4((light.ulBasePath + "params").c_str(), light.params);
        light.UnlockRead();
    }

    void LightSystem::m_UpdateStatusUL(LightComponent& light, std::shared_ptr<ShaderManager> pShader)
    {
        light.LockRead();
        pShader->SetUniformInt((light.ulBasePath + "status").c_str(), light.status);
        light.UnlockRead();
    }
}
