#include "pch.h"

#include "TextureManager.h"

#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include "Engine/Utils/Math.h"
#include "Engine/Utils/TextureUtils.h"

#include <sstream>
#include "TextureManager.h"

#define MAX_SAMPLER 20
#define MAX_COLOR_SAMPLER 4

namespace MyEngine
{
	void TextureManager::SetBasePath(const std::string& basePath)
	{
		m_basePath = basePath;
	}

	size_t TextureManager::LoadResource(const std::string& name)
	{
		std::shared_ptr<iResource> pResource = GetResource(name);
		if (pResource)
		{
			return pResource->index;
		}

		std::shared_ptr<sTextureInfo> pTexture(new sTextureInfo());

		m_mapTextures[name] = pTexture;
		pTexture->index = m_vecTextures.size();
		m_vecTextures.push_back(pTexture);

		return pTexture->index;
	}
	
	void TextureManager::DeleteResource(const std::string& name)
	{
		std::shared_ptr<iResource> pTexture = GetResource(name);
		if (!pTexture)
		{
			return;
		}

		m_mapTextures.erase(name);
		m_vecTextures.erase(m_vecTextures.begin() + pTexture->index);
	}
	
	void TextureManager::DeleteResource(const size_t& index)
	{
		std::shared_ptr<iResource> pTexture = GetResource(index);
		if (!pTexture)
		{
			return;
		}

		m_mapTextures.erase(pTexture->name);
		m_vecTextures.erase(m_vecTextures.begin() + index);
	}
	
	void TextureManager::ClearResources()
	{
		for (size_t i = 0; i < m_vecTextures.size();)
		{
			DeleteResource(i);
		}
	}
	
	std::shared_ptr<iResource> TextureManager::GetResource(size_t index)
	{
		if (index < m_vecTextures.size())
		{
			return m_vecTextures.at(index);
		}

		// Not found
		return nullptr;
	}
	
	std::shared_ptr<iResource> TextureManager::GetResource(const std::string& name)
	{
		if (m_mapTextures.find(name) != m_mapTextures.end())
		{
			return m_mapTextures.at(name);
		}

		// Not found
		return nullptr;
	}
	
	std::vector<std::shared_ptr<iResource>>& TextureManager::GetResources()
	{
		return m_vecTextures;
	}
	
	void TextureManager::ActivateResource(const std::string& name)
	{
		int unitId = GL_TEXTURE0;
		std::shared_ptr<sTextureInfo> pTexture = std::static_pointer_cast<sTextureInfo>(GetResource(name));
		std::shared_ptr<sSamplerInfo> pSampler = GetSampler(pTexture->type);

		unitId += pSampler->samplerId;

		glActiveTexture(unitId);

		if (pTexture->type == eTextureType::CUBE)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, pTexture->textNumber);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, pTexture->textNumber);
		}

		m_currTexture = name;

		// Bind sampler and uniforms
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		pShader->SetUniformInt(pSampler->toggle.c_str(), true);
		pShader->SetUniformInt(pSampler->name.c_str(), pSampler->samplerId);

		return;
	}

	void TextureManager::ActivateResource(const std::string& name, const float& ratio)
	{
		int unitId = GL_TEXTURE0;
		std::shared_ptr<sTextureInfo> pTexture = std::static_pointer_cast<sTextureInfo>(GetResource(name));
		std::shared_ptr<sSamplerInfo> pSampler = GetSampler(pTexture->type);

		unitId += pSampler->samplerId;

		glActiveTexture(unitId);

		if (pTexture->type == eTextureType::CUBE)
		{
			glBindTexture(GL_TEXTURE_CUBE_MAP, pTexture->textNumber);
		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, pTexture->textNumber);
		}

		m_currTexture = name;

		// Bind sampler and uniforms
		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		pShader->SetUniformInt(pSampler->toggle.c_str(), true);
		pShader->SetUniformInt(pSampler->name.c_str(), pSampler->samplerId);
		pShader->SetUniformFloat(pSampler->ratio.c_str(), ratio);
	}
	
	void TextureManager::DeactivateResource()
	{
		m_nextColorSamplerId = 0;

		std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		for (std::pair<GLuint, std::shared_ptr<sSamplerInfo>> pairSampler : m_mapIdSampler)
		{
			std::shared_ptr<sSamplerInfo> pSampler = pairSampler.second;
			pShader->SetUniformInt(pSampler->toggle.c_str(), false);
		}
	}
	
	std::shared_ptr<iResource> TextureManager::GetActiveResource()
	{
		return GetResource(m_currTexture);
	}

	bool TextureManager::Create2DTexture(const std::string& name, const eTextureType& type)
	{
		std::string fileToLoadFullPath = m_basePath + name;
		std::shared_ptr<sTextureInfo> pTexture = nullptr;
		std::shared_ptr<iResource> pResource = GetResource(name);

		if (pResource)
		{
			pTexture = std::static_pointer_cast<sTextureInfo>(pResource);
			if (pTexture->textNumber > 0)
			{
				// Texture already created and binded
				return true;
			}
		}
		else
		{
			size_t index = LoadResource(name);
			pResource = GetResource(index);
			pTexture = std::static_pointer_cast<sTextureInfo>(pResource);
		}

		bool isTextCreated = TextureUtils::CreateTextureFromBMPFile(name, fileToLoadFullPath, true, pTexture);
		if (!isTextCreated)
		{
			return false;
		}

		pTexture->type = type;

		return true;
	}

	bool TextureManager::CreateCubeTexture(const std::string& cubeMapName,
											const std::string& posX_fileName, const std::string& negX_fileName,
											const std::string& posY_fileName, const std::string& negY_fileName,
											const std::string& posZ_fileName, const std::string& negZ_fileName)
	{
		std::shared_ptr<sTextureInfo> pTexture = nullptr;
		std::shared_ptr<iResource> pResource = GetResource(cubeMapName);

		if (pResource)
		{
			pTexture = std::static_pointer_cast<sTextureInfo>(pResource);
			if (pTexture->textNumber > 0)
			{
				// Texture already created and binded
				return true;
			}
		}
		else
		{
			size_t index = LoadResource(cubeMapName);
			pResource = GetResource(index);
			pTexture = std::static_pointer_cast<sTextureInfo>(pResource);
		}

		std::string posX_fileName_FullPath = m_basePath + "/" + posX_fileName;
		std::string negX_fileName_FullPath = m_basePath + "/" + negX_fileName;
		std::string posY_fileName_FullPath = m_basePath + "/" + posY_fileName;
		std::string negY_fileName_FullPath = m_basePath + "/" + negY_fileName;
		std::string posZ_fileName_FullPath = m_basePath + "/" + posZ_fileName;
		std::string negZ_fileName_FullPath = m_basePath + "/" + negZ_fileName;

		bool cubeCreated = TextureUtils::CreateCubeTextureFromBMPFiles(cubeMapName,
													posX_fileName_FullPath, negX_fileName_FullPath,
													posY_fileName_FullPath, negY_fileName_FullPath,
													posZ_fileName_FullPath, negZ_fileName_FullPath, 
													true, pTexture);
		if (!cubeCreated)
		{
			return false;
		}

		pTexture->type = eTextureType::CUBE;

		return true;
	}

	std::shared_ptr<sSamplerInfo> TextureManager::GetSampler(const eTextureType& textureType)
	{
		GLuint samplerId = 0;

		if (textureType == eTextureType::COLOR)
		{
			samplerId = m_nextColorSamplerId;

			m_nextColorSamplerId = (m_nextColorSamplerId + 1) % MAX_COLOR_SAMPLER;
		}
		else
		{
			samplerId = static_cast<GLuint>(textureType);
		}

		assert(m_mapIdSampler.find(samplerId) != m_mapIdSampler.end());

		return m_mapIdSampler.at(samplerId);
	}

	std::shared_ptr<sSamplerInfo> TextureManager::GetSampler(const std::string& textureName)
	{
		std::shared_ptr<sTextureInfo> pTexture = std::static_pointer_cast<sTextureInfo>(GetResource(textureName));

		return GetSampler(pTexture->type);
	}

	void TextureManager::m_CreateSamplers()
	{
		// Color samplers
		for (int i = 0; i < 4; i++)
		{
			std::shared_ptr<sSamplerInfo> pColorSampler(new sSamplerInfo("colorTexture0" + std::to_string(i),
																	"colorTextureRatio0" + std::to_string(i),
																	"bUseColorTexture",
																	static_cast<GLuint>(i)));

			m_mapIdSampler[i] = pColorSampler;
		}

		// Height map
		std::shared_ptr<sSamplerInfo> pHeightSampler(new sSamplerInfo("heightMapTexture",
																"heightScale",
																"bUseHeightMapTexture",
															    static_cast<GLuint>(eTextureType::HEIGHTMAP)));

		m_mapIdSampler[eTextureType::HEIGHTMAP] = pHeightSampler;

		// Alpha
		std::shared_ptr<sSamplerInfo> pAlphaSampler(new sSamplerInfo("alphaTexture",
																"",
																"bUseAlphaTexture",
															    static_cast<GLuint>(eTextureType::TRANSPARENCY)));

		m_mapIdSampler[eTextureType::TRANSPARENCY] = pAlphaSampler;

		// Normal
		std::shared_ptr<sSamplerInfo> pNormalSampler(new sSamplerInfo("normalTexture",
																"",
																"bUseNormalTexture",
															    static_cast<GLuint>(eTextureType::NORMAL)));

		m_mapIdSampler[eTextureType::NORMAL] = pNormalSampler;

		// Specular
		std::shared_ptr<sSamplerInfo> pSpecularSampler(new sSamplerInfo("specularTexture",
																"",
																"bUseSpecularTexture",
															    static_cast<GLuint>(eTextureType::SPECULAR)));

		m_mapIdSampler[eTextureType::SPECULAR] = pSpecularSampler;

		// Discard
		std::shared_ptr<sSamplerInfo> pDiscardSampler(new sSamplerInfo("discardTexture",
																"",
																"bUseDiscardTexture",
															    static_cast<GLuint>(eTextureType::DISCARD)));

		m_mapIdSampler[eTextureType::DISCARD] = pDiscardSampler;

		// Cube
		std::shared_ptr<sSamplerInfo> pCubeSampler(new sSamplerInfo("cubeTexture",
																"",
																"bUseCubeTexture",
															    static_cast<GLuint>(eTextureType::CUBE)));

		m_mapIdSampler[eTextureType::CUBE] = pCubeSampler;
	}
}
