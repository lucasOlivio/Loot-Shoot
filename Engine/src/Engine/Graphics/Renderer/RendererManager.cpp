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
	/*using itRenderInfos = std::map<std::string, std::vector<sRenderInfo>>::iterator;
	using pairRenderInfos = std::pair<std::string, std::vector<sRenderInfo>>;*/

	RendererManager::RendererManager()
	{
	}

	void RendererManager::AddToRender(const sRenderModelInfo& renderInfo)
	{
		m_vecRenderModelInfos.push_back(renderInfo);
	}

	void RendererManager::RenderAll(std::shared_ptr<Scene> pScene)
	{
		m_UpdateCamera(pScene);

		// TODO: Join this with the instancing stuff
		// Render all models
		std::shared_ptr<iResourceManager> pMaterialManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MATERIAL);
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

		pShader->ActivateResource(DEFAULT_SHADER);
		for (const sRenderModelInfo& renderInfo : m_vecRenderModelInfos)
		{
			pMaterialManager->ActivateResource(renderInfo.materialName);

			GraphicsUtils::DrawModel(renderInfo, pShader);
		}

		// Render particles using instancing
		pShader->ActivateResource(INSTANCING_SHADER);
		//for (const pairRenderInfos& pair : m_mapRenderInfos)
		//{
		//	std::shared_ptr<iResourceManager> pMeshManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MESH);
		//	std::shared_ptr<iResource> pResource = pMeshManager->GetResource(pair.first);
		//	std::shared_ptr<sMeshInfo> pMesh = std::static_pointer_cast<sMeshInfo>(pResource);
		//	
		//	/*GraphicsUtils::DrawInstanced(m_bufferId, pMesh->VAO_ID, 
		//								 pMesh->numberOfIndices, pair.second);*/
		//}

		pShader->ActivateResource(DEFAULT_SHADER);
	}

	void RendererManager::ClearRender()
	{
		m_vecRenderModelInfos.clear();

		/*for (itRenderInfos it = m_mapRenderInfos.begin(); it != m_mapRenderInfos.end(); it++)
		{
			it->second.clear();
		}*/
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