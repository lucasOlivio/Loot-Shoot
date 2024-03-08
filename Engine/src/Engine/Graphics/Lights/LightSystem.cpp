#include "pch.h"

#include "LightSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

namespace MyEngine
{
    void LightSystem::Init()
    {
    }

    void LightSystem::Start(std::shared_ptr<Scene> pScene)
    {
        // Go over each lights setting the ULs and initializing them
        int lightIndex = 0;
        for (Entity entityId : SceneView<TransformComponent, LightComponent>(*pScene))
        {
            LightComponent& light = pScene->Get<LightComponent>(entityId);
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);

            m_SetupLight(transform, light, lightIndex);
            lightIndex++;
        }
    }

    void LightSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<iShaderProgram> pShader = ShaderManager::GetActiveShader();

        // Set lights to transform position of entity
        for (Entity entityId : SceneView<TransformComponent, LightComponent>(*pScene))
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

    void LightSystem::m_SetupLight(TransformComponent& transform,
                                   LightComponent& light, int lightIndex)
    {
        std::string ulBasePath = "theLights[" + std::to_string(lightIndex) + "].";
        std::shared_ptr<iShaderProgram> pShader = ShaderManager::GetActiveShader();

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
                                         std::shared_ptr<iShaderProgram> pShader)
    {
        glm::vec4 newPosition = glm::vec4(transform.worldPosition, 0) + light.positionOffset +
                               (light.direction * light.directionOffset);

        pShader->SetUniformVec4((light.ulBasePath + "position").c_str(), newPosition);
    }

    void LightSystem::m_UpdateDirectionUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader)
    {
        glm::vec4 newDirection = light.direction + light.directionOffset;
        pShader->SetUniformVec4((light.ulBasePath + "direction").c_str(), newDirection);
    }

    void LightSystem::m_UpdateDiffuseUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader)
    {
        pShader->SetUniformVec4((light.ulBasePath + "diffuse").c_str(), light.diffuse);
    }

    void LightSystem::m_UpdateSpecularUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader)
    {
        pShader->SetUniformVec4((light.ulBasePath + "specular").c_str(), light.specular);
    }

    void LightSystem::m_UpdateAttenUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader)
    {
        glm::vec4 newAtten = light.atten * light.flickerOffset;
        pShader->SetUniformVec4((light.ulBasePath + "atten").c_str(), newAtten);
    }

    void LightSystem::m_UpdateParamsUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader)
    {
        pShader->SetUniformVec4((light.ulBasePath + "params").c_str(), light.params);
    }

    void LightSystem::m_UpdateStatusUL(LightComponent& light, std::shared_ptr<iShaderProgram> pShader)
    {
        pShader->SetUniformInt((light.ulBasePath + "status").c_str(), light.status);
    }
}
