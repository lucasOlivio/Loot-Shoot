#include "pch.h"

#include "RenderSystem.h"

#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Materials/Material.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"

#include "Engine/ECS/Scene/SceneView.hpp"

#include "Engine/Graphics/Components/GraphicsLocator.h"
#include "Engine/Graphics/Components/Components.h"
#include "Engine/Graphics/Renderer/RendererManagerLocator.h"
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
		std::shared_ptr<TextureManager> pTextureManager = ResourceManagerFactory::GetOrCreate<TextureManager>(eResourceTypes::TEXTURE);

		// Load textures
		pTextureManager->SetBasePath(pConfigPath->pathTextures);
		for (Entity entityId : SceneView<TextureComponent>(*pScene))
		{
			TextureComponent& texture = pScene->Get<TextureComponent>(entityId);
			std::string errorMsg = "";
			bool isLoaded = true;

			texture.LockRead();
			if (texture.textureType == eTextureType::CUBE)
			{
				isLoaded = pTextureManager->CreateCubeTexture(texture.fileName,
															texture.vecTextures[0],
															texture.vecTextures[1],
															texture.vecTextures[2],
															texture.vecTextures[3],
															texture.vecTextures[4],
															texture.vecTextures[5]);
				texture.UnlockRead();
				continue;
			}
			else
			{
				isLoaded = pTextureManager->Create2DTexture(texture.fileName, texture.textureType);
			}

			if (!isLoaded)
			{
				LOG_ERROR("Error loading texture: " + texture.fileName);
				texture.UnlockRead();
				continue;
			}
			texture.UnlockRead();
		}

		// Load Models
		std::shared_ptr<iResourceManager> pMeshManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MESH);
		for (Entity entityId : SceneView<ModelComponent>(*pScene))
		{
			ModelComponent& model = pScene->Get<ModelComponent>(entityId);
			
			model.LockWrite();
			size_t index = pMeshManager->LoadResource(model.model);
			model.pMesh = std::static_pointer_cast<sMeshInfo>(pMeshManager->GetResource(index));
			model.UnlockWrite();
		}

		// Load Materials
		std::shared_ptr<iResourceManager> pMaterialManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MATERIAL);
		for (Entity entityId : SceneView<MaterialComponent>(*pScene))
		{
			MaterialComponent& material = pScene->Get<MaterialComponent>(entityId);

			material.LockRead();
			size_t index = pMaterialManager->LoadResource(material.name);

			std::shared_ptr<sMaterialInfo> pMaterial = std::static_pointer_cast<sMaterialInfo>(pMaterialManager->GetResource(material.name));
			pMaterial->name = material.name;
			pMaterial->alphaTexture = material.alphaTexture;
			pMaterial->cubeTexture = material.cubeTexture;
			pMaterial->discardTexture = material.discardTexture;
			pMaterial->specularTexture = material.specularTexture;
			pMaterial->normalTexture = material.normalTexture;
			pMaterial->heightMapTexture = material.heightMapTexture;
			pMaterial->colorTextures = material.colorTextures;
			pMaterial->offset = material.offset;
			pMaterial->currOffset = material.currOffset;
			pMaterial->offsetMove = material.offsetMove;
			pMaterial->currOffsetHeightMap = material.currOffsetHeightMap;
			pMaterial->offsetHeightMap = material.offsetHeightMap;
			pMaterial->colorTexturesRatios = material.colorTexturesRatios;
			pMaterial->heightScale = material.heightScale;
			pMaterial->useHeightMap = material.useHeightMap;
			pMaterial->useNormalTexture = material.useNormalTexture;
			pMaterial->useSpecularTexture = material.useSpecularTexture;
			pMaterial->useDiscardTexture = material.useDiscardTexture;
			pMaterial->useCubeTexture = material.useCubeTexture;
			pMaterial->isEmissive = material.isEmissive;
			pMaterial->useAlphaTexture = material.useAlphaTexture;
			material.UnlockRead();
		}
    }

    void RenderSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void RenderSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void RenderSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

	void RenderSystem::Shutdown()
	{
	}
}
