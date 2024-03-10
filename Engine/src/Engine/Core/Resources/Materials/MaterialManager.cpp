#include "pch.h"

#include "MaterialManager.h"

#include "Engine/Core/Resources/Materials/Material.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"

#include "Engine/ECS/Scene/SceneView.hpp"

namespace MyEngine
{
	using itMaterials = std::map<std::string, sMaterialInfo>::iterator;

	size_t MaterialManager::LoadResource(const std::string& name)
	{
		size_t index = m_vecMaterials.size();

		std::shared_ptr<sMaterialInfo> newMat = std::shared_ptr<sMaterialInfo>(new sMaterialInfo());
		m_mapMaterials[name] = newMat;
		m_vecMaterials.push_back(newMat);

		return index;
	}

	void MaterialManager::DeleteResource(const std::string& name)
	{
		std::shared_ptr<iResource> pMaterial = m_mapMaterials[name];

		m_mapMaterials.erase(name);
		m_vecMaterials.erase(m_vecMaterials.begin() + pMaterial->index);
	}

	void MaterialManager::DeleteResource(const size_t& index)
	{
		std::shared_ptr<iResource> pMaterial = m_vecMaterials[index];

		m_mapMaterials.erase(pMaterial->name);
		m_vecMaterials.erase(m_vecMaterials.begin() + index);
	}

	void MaterialManager::SetBasePath(const std::string& basePath)
	{
		// Materials dont use folder paths for now
	}

	void MaterialManager::ClearResources()
	{
		m_mapMaterials.clear();
		m_vecMaterials.clear();
	}

	std::shared_ptr<iResource> MaterialManager::GetResource(size_t index)
	{
		return m_vecMaterials.at(index);
	}

	std::shared_ptr<iResource> MaterialManager::GetResource(const std::string& name)
	{
		return m_mapMaterials.at(name);
	}

	std::vector<std::shared_ptr<iResource>>& MaterialManager::GetResources()
	{
		return m_vecMaterials;
	}

	void MaterialManager::ActivateResource(const std::string& name)
	{
		if (name == "")
		{
			DeactivateResource();
			return;
		}

		std::shared_ptr<sMaterialInfo> pMaterial = std::static_pointer_cast<sMaterialInfo>(GetResource(name));

		// Only change material if not already binded
		if (pMaterial->name == m_currMaterial)
		{
			return;
		}
		DeactivateResource();

		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		std::shared_ptr<TextureManager> pTextureManager = ResourceManagerFactory::GetOrCreate<TextureManager>(eResourceTypes::TEXTURE);

		// TODO: Regulate the intensity of the emissiviness
		pShader->SetUniformInt("isEmissive", pMaterial->isEmissive);

		glm::vec3 finaloffset = pMaterial->currOffset + pMaterial->offset;
		pShader->SetUniformVec2("UVOffset", finaloffset);

		pShader->SetUniformVec2("HeightMapOffset", pMaterial->currOffsetHeightMap);

		// Bind color textures
		for (int i = 0; i < pMaterial->colorTextures.size(); i++)
		{
			pTextureManager->ActivateResource(pMaterial->colorTextures[i], pMaterial->colorTexturesRatios[i]);
		}

		// TODO: Remove repetition, all could be in a vector or map
		// Bind heightmap textures
		if (pMaterial->useHeightMap)
		{
			// Height scale can be passed as the "ratio parameter", will be placed on the heightscale uniform
			pTextureManager->ActivateResource(pMaterial->heightMapTexture, pMaterial->heightScale);
		}

		// Bind normal textures
		if (pMaterial->useNormalTexture)
		{
			pTextureManager->ActivateResource(pMaterial->normalTexture);
		}

		// Bind specular textures
		if (pMaterial->useSpecularTexture)
		{
			pTextureManager->ActivateResource(pMaterial->specularTexture);
		}

		// Bind discard textures
		if (pMaterial->useDiscardTexture)
		{
			pTextureManager->ActivateResource(pMaterial->discardTexture);
		}

		// Bind alpha textures
		if (pMaterial->useAlphaTexture)
		{
			pTextureManager->ActivateResource(pMaterial->alphaTexture);
		}

		// Bind cube textures
		if (pMaterial->useCubeTexture)
		{
			pTextureManager->ActivateResource(pMaterial->cubeTexture);
		}

		m_currMaterial = pMaterial->name;
	}

	void MaterialManager::DeactivateResource()
	{
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		std::shared_ptr<iResourceManager> pTextureManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::TEXTURE);

		pTextureManager->DeactivateResource();
		pShader->SetUniformVec2("UVOffset", glm::vec2(0.0, 0.0));
		pShader->SetUniformVec2("HeightMapOffset", glm::vec2(0.0, 0.0));
		pShader->SetUniformInt("isEmissive", false);

		m_currMaterial = "";
	}

	std::shared_ptr<iResource> MaterialManager::GetActiveResource()
	{
		return m_mapMaterials.at(m_currMaterial);
	}
}