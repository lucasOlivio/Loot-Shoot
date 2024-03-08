#include "pch.h"

#include "RenderSystem.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"
#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/ECS/Components.h"

#include "Engine/Graphics/Shaders/ShaderManager.h"
#include "Engine/Graphics/Renderer/RendererManagerLocator.h"
#include "Engine/Graphics/Textures/TextureManagerLocator.h"
#include "Engine/Graphics/Materials/MaterialManagerLocator.h"
#include "Engine/Graphics/VAO/VAOManagerLocator.h"
#include "Engine/Graphics/GraphicsProperties.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/GraphicsUtils.h"

namespace MyEngine
{
	void RenderSystem::Init()
	{
	}

	void RenderSystem::Start(std::shared_ptr<Scene> pScene)
    {
		std::shared_ptr<ConfigPathComponent> pConfigPath = CoreLocator::GetConfigPath();
		std::shared_ptr<iTextureManager> pTextureManager = TextureManagerLocator::Get();
		std::shared_ptr<iMaterialManager> pMaterialManager = MaterialManagerLocator::Get();
		std::shared_ptr<iVAOManager> pVAOManager = VAOManagerLocator::Get();

		// Load textures
		pTextureManager->SetBasePath(pConfigPath->pathTextures);
		for (Entity entityId : SceneView<TextureComponent>(*pScene))
		{
			TextureComponent& texture = pScene->Get<TextureComponent>(entityId);
			std::string errorMsg = "";
			bool isLoaded = true;

			if (texture.textureType == eTextureType::CUBE)
			{
				isLoaded = pTextureManager->CreateCubeTextureFromBMPFiles(texture.fileName,
																		  texture.vecTextures[0],
																		  texture.vecTextures[1],
																		  texture.vecTextures[2],
																		  texture.vecTextures[3],
																		  texture.vecTextures[4],
																		  texture.vecTextures[5],
																		  true,
																		  errorMsg);
				continue;
			}
			else
			{
				isLoaded = pTextureManager->Create2DTextureFromBMPFile(texture.fileName, true);
				errorMsg = pTextureManager->GetLastError();
			}

			if (!isLoaded)
			{
				LOG_ERROR(errorMsg);
				continue;
			}
		}

		// Load Models
		pVAOManager->SetBasePath(pConfigPath->pathModels);
		for (Entity entityId : SceneView<ModelComponent>(*pScene))
		{
			ModelComponent& model = pScene->Get<ModelComponent>(entityId);
			model.pMeshes.resize(model.models.size(), nullptr);
			for (int i = 0; i < model.models.size(); i++)
			{
				sMesh* pMesh = pVAOManager->LoadModelIntoVAO(model.models[i], model.isDynamic);
				if (!pMesh)
				{
					continue;
				}
				model.pMeshes[i] = pMesh;
			}
		}

		// Load Materials
		for (Entity entityId : SceneView<MaterialComponent>(*pScene))
		{
			MaterialComponent& material = pScene->Get<MaterialComponent>(entityId);

			sMaterialInfo materialInfo = sMaterialInfo();
			materialInfo.name = material.name;
			materialInfo.alphaTexture = material.alphaTexture;
			materialInfo.cubeTexture = material.cubeTexture;
			materialInfo.discardTexture = material.discardTexture;
			materialInfo.specularTexture = material.specularTexture;
			materialInfo.normalTexture = material.normalTexture;
			materialInfo.heightMapTexture = material.heightMapTexture;
			materialInfo.colorTextures = material.colorTextures;
			materialInfo.offset = material.offset;
			materialInfo.currOffset = material.currOffset;
			materialInfo.offsetMove = material.offsetMove;
			materialInfo.currOffsetHeightMap = material.currOffsetHeightMap;
			materialInfo.offsetHeightMap = material.offsetHeightMap;
			materialInfo.colorTexturesRatios = material.colorTexturesRatios;
			materialInfo.heightScale = material.heightScale;
			materialInfo.useHeightMap = material.useHeightMap;
			materialInfo.useNormalTexture = material.useNormalTexture;
			materialInfo.useSpecularTexture = material.useSpecularTexture;
			materialInfo.useDiscardTexture = material.useDiscardTexture;
			materialInfo.useCubeTexture = material.useCubeTexture;
			materialInfo.isEmissive = material.isEmissive;
			materialInfo.useAlphaTexture = material.useAlphaTexture;

			pMaterialManager->LoadMaterial(material.name, materialInfo);
		}
    }

    void RenderSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void RenderSystem::Render(std::shared_ptr<Scene> pScene)
    {
		std::shared_ptr<iRendererManager> pRenderer = RendererManagerLocator::Get();
		std::shared_ptr<iShaderProgram> pShader = ShaderManager::GetActiveShader();
		std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

		pRenderer->RenderAllModels(pScene);
		pRenderer->ClearRender();
    }

    void RenderSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

	void RenderSystem::Shutdown()
	{
	}
}
