#include "pch.h"

#include "RendererManager.h"

#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"
#include "Engine/ECS/Components.h"

#include "Engine/Graphics/Materials/MaterialManagerLocator.h"
#include "Engine/Graphics/Shaders/ShaderManager.h"

#include "Engine/Utils/CameraUtils.h"
#include "Engine/Utils/GraphicsUtils.h"

namespace MyEngine
{
	using pairFBOInfos = std::pair<uint, std::vector<sRenderModelInfo>>;
	using itFBOInfos = std::map<uint, std::vector<sRenderModelInfo>>::iterator;

	RendererManager::RendererManager()
	{
	}

	void RendererManager::AddToRender(const sRenderModelInfo& renderInfo)
	{
		m_vecRenderInfos.push_back(renderInfo);
	}

	void RendererManager::RenderAllModels(std::shared_ptr<Scene> pScene)
	{
		std::shared_ptr<iMaterialManager> pMaterialManager = MaterialManagerLocator::Get();

		// Calculate projection and view mats and bind to shader
		std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();
		std::shared_ptr<iShaderProgram> pShader = ShaderManager::GetActiveShader();
		CameraComponent& camera = pScene->Get<CameraComponent>(CAMERA_ID);
		TransformComponent& transformCamera = pScene->Get<TransformComponent>(CAMERA_ID);

		glm::mat4 matProjection = CameraUtils::ProjectionMat(camera.fovy, camera.zNear,
															camera.zFar, pWindow->width, pWindow->height);
		pShader->SetUniformMatrix4f("matProjection", matProjection);

		glm::mat4 matView = CameraUtils::ViewMat(transformCamera.position, transformCamera.orientation,
												camera.distance, camera.height, camera.offsetTarget);
												pShader->SetUniformMatrix4f("matView", matView);

		// Render all models from list models
		m_RenderList(pMaterialManager, m_vecRenderInfos);
	}

	void RendererManager::ClearRender()
	{
		m_vecRenderInfos.clear();
	}

	void RendererManager::m_RenderList(std::shared_ptr<iMaterialManager> pMaterialManager,
									   const std::vector<sRenderModelInfo>& renderInfos)
	{
		for (const sRenderModelInfo& renderInfo : renderInfos)
		{
			pMaterialManager->BindMaterial(renderInfo.materialName);

			GraphicsUtils::DrawModel(renderInfo);
		}
	}
}