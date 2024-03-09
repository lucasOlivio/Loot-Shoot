#include "pch.h"

#include "MaterialManager.h"

#include "Engine/Core/Resources/Materials/Material.h"

#include "Engine/Graphics/Shaders/ShaderManager.h"
#include "Engine/Graphics/Textures/TextureManagerLocator.h"

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

		std::shared_ptr<iShaderProgram> pShader = ShaderManager::GetActiveShader();
		std::shared_ptr<iTextureManager> pTextureManager = TextureManagerLocator::Get();

		// Update offset, alpha and if is emissive material
		// TODO: Regulate the intensity of the emissiviness
		pShader->SetUniformInt("isEmissive", pMaterial->isEmissive);

		glm::vec3 finaloffset = pMaterial->currOffset + pMaterial->offset;
		pShader->SetUniformVec2("UVOffset", finaloffset);

		pShader->SetUniformVec2("HeightMapOffset", pMaterial->currOffsetHeightMap);

		// Bind color textures
		for (int i = 0; i < pMaterial->colorTextures.size(); i++)
		{
			pTextureManager->BindTexture(pMaterial->colorTextures[i],
				eTextureType::COLOR,
				pMaterial->colorTexturesRatios[i]);
		}

		// TODO: Remove repetition, all could be in a vector or map
		// Bind heightmap textures
		if (pMaterial->useHeightMap)
		{
			// Height scale can be passed as the "ratio parameter", will be placed on the heightscale uniform
			pTextureManager->BindTexture(pMaterial->heightMapTexture,
				eTextureType::HEIGHTMAP,
				pMaterial->heightScale);
		}

		// Bind normal textures
		if (pMaterial->useNormalTexture)
		{
			pTextureManager->BindTexture(pMaterial->normalTexture,
				eTextureType::NORMAL,
				0);
		}

		// Bind specular textures
		if (pMaterial->useSpecularTexture)
		{
			pTextureManager->BindTexture(pMaterial->specularTexture,
				eTextureType::SPECULAR,
				0);
		}

		// Bind discard textures
		if (pMaterial->useDiscardTexture)
		{
			pTextureManager->BindTexture(pMaterial->discardTexture,
				eTextureType::DISCARD,
				0);
		}

		// Bind alpha textures
		if (pMaterial->useAlphaTexture)
		{
			pTextureManager->BindTexture(pMaterial->alphaTexture,
				eTextureType::TRANSPARENCY,
				0);
		}

		// Bind cube textures
		if (pMaterial->useCubeTexture)
		{
			pTextureManager->BindTexture(pMaterial->cubeTexture,
				eTextureType::CUBE,
				0);
		}

		m_currMaterial = pMaterial->name;
	}

	void MaterialManager::DeactivateResource()
	{
		std::shared_ptr<iShaderProgram> pShader = ShaderManager::GetActiveShader();
		std::shared_ptr<iTextureManager> pTextureManager = TextureManagerLocator::Get();

		pTextureManager->ResetSamplers();
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
