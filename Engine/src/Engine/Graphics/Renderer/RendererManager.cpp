#include "pch.h"

#include "RendererManager.h"

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Components/SingletonComponents.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include "Engine/Core/Resources/ResourceManagerFactory.h"

#include "Engine/Graphics/Components/GraphicsLocator.h"

#include "Engine/Utils/CameraUtils.h"
#include "Engine/Utils/GraphicsUtils.h"

namespace MyEngine
{
	using pairFBOInfos = std::pair<uint, std::vector<sRenderModelInfo>>;
	using itFBOInfos = std::map<uint, std::vector<sRenderModelInfo>>::iterator;

	RendererManager::RendererManager()
	{
	}

	void RendererManager::AddToRender(const sRenderParticleInfo& renderInfo)
	{
		m_vecRenderParticleInfos.push_back(renderInfo);
	}

	void RendererManager::AddToRender(const sRenderModelInfo& renderInfo)
	{
		m_vecRenderInfos.push_back(renderInfo);
	}

	void RendererManager::RenderAll(std::shared_ptr<Scene> pScene)
	{
		m_UpdateCamera(pScene);

		m_RenderList(m_vecRenderInfos);

		m_RenderList(m_vecRenderParticleInfos);
	}

	void RendererManager::ClearRender()
	{
		m_vecRenderInfos.clear();
		m_vecRenderParticleInfos.clear();
	}

	void RendererManager::m_RenderList(const std::vector<sRenderModelInfo>& renderInfos)
	{
		std::shared_ptr<iResourceManager> pMaterialManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MATERIAL);
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		
		for (const sRenderModelInfo& renderInfo : renderInfos)
		{
			pMaterialManager->ActivateResource(renderInfo.materialName);

			GraphicsUtils::DrawModel(renderInfo, pShader);
		}
	}

	void RendererManager::m_RenderList(const std::vector<sRenderParticleInfo>& renderInfos)
	{
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

		for (const sRenderParticleInfo& renderInfo : renderInfos)
		{
			GraphicsUtils::DrawParticle(renderInfo, pShader);
		}
	}

	void RendererManager::m_UpdateCamera(std::shared_ptr<Scene> pScene)
	{
		std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

		CameraComponent& camera = pScene->Get<CameraComponent>(CAMERA_ID);
		TransformComponent& transformCamera = pScene->Get<TransformComponent>(CAMERA_ID);

		glm::mat4 matProjection = CameraUtils::ProjectionMat(camera.fovy, camera.zNear,
															 camera.zFar, pWindow->width, pWindow->height);
		glm::mat4 matView = CameraUtils::ViewMat(transformCamera.position, transformCamera.orientation,
												 camera.distance, camera.height, camera.offsetTarget);

		pShader->SetUniformMatrix4f("matProjection", matProjection);
		pShader->SetUniformMatrix4f("matView", matView);
	}
}