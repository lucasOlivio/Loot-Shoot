#include "pch.h"

#include "ModelRenderSystem.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Graphics/Renderer/RendererManagerLocator.h"
#include "Engine/Graphics/Components/Components.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/GraphicsUtils.h"

namespace MyEngine
{
    void ModelRenderSystem::Init()
    {
    }

    void ModelRenderSystem::Start(std::shared_ptr<Scene> pScene)
    {
        EntitySystem::Start(pScene);
    }

    void ModelRenderSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void ModelRenderSystem::Render(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<iRendererManager> pRendererManager = RendererManagerLocator::Get();

        for (Entity entityId : m_vecEntities)
        {
            TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
            ModelComponent& model = pScene->Get<ModelComponent>(entityId);
            if (!model.isActive)
            {
                continue;
            }

            glm::mat4 matTransform = glm::mat4(1.0);

            TransformUtils::GetTransform(transform.worldPosition,
                                         transform.worldOrientation,
                                         transform.worldScale,
                                         matTransform);

            std::shared_ptr<sMeshInfo> pMesh = model.pMesh;
            if (!pMesh)
            {
                continue;
            }

            sRenderModelInfo renderInfo = sRenderModelInfo();
            renderInfo.materialName = model.material;
            renderInfo.matModel = matTransform;
            renderInfo.defaultColor = model.defaultColor;
            renderInfo.VAO_ID = pMesh->VAO_ID;
            renderInfo.numberOfIndices = pMesh->numberOfIndices;
            // This all should come from material
            renderInfo.isWireFrame = model.isWireframe;
            renderInfo.doNotLight = model.doNotLight;
            renderInfo.useDefaultColor = model.useDefaultColor;
            renderInfo.useColorTexture = model.useColorTexture;
            renderInfo.useDebugColor = false;

            bool hasTiling = false;
            TilingComponent& tiling = pScene->Get<TilingComponent>(entityId, hasTiling);
            if (hasTiling)
            {
                renderInfo.tileAxis = tiling.axis;
                renderInfo.tileOffset = tiling.offset;
            }

            pRendererManager->AddToRender(renderInfo);
        }
    }

    void ModelRenderSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void ModelRenderSystem::Shutdown()
    {
    }

    void ModelRenderSystem::SetSystemMask(std::shared_ptr<Scene> pScene)
    {
        ComponentType transformType = pScene->GetComponentType<TransformComponent>();
        ComponentType modelType = pScene->GetComponentType<ModelComponent>();

        m_systemMask.set(transformType);
        m_systemMask.set(modelType);
    }
}
