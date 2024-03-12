#pragma once

#include "Engine/Core/Resources/iResourceManager.h"
#include "Engine/Core/Resources/Textures/Texture.h"

#include "Engine/Singleton.hpp"

#include <map>

namespace MyEngine
{
	class TextureManager : public Singleton<TextureManager>,
							public iResourceManager
	{
		friend class Singleton<TextureManager>;
	public:
		virtual void SetBasePath(const std::string& basePath);
		virtual size_t LoadResource(const std::string& name);

		virtual void DeleteResource(const std::string& name);
		virtual void DeleteResource(const size_t& index);
		virtual void ClearResources();

		virtual std::shared_ptr<iResource> GetResource(size_t index);
		virtual std::shared_ptr<iResource> GetResource(const std::string& name);
		virtual std::vector<std::shared_ptr<iResource>>& GetResources();

		virtual void ActivateResource(const std::string& name);
		virtual void ActivateResource(const std::string& name, const float& ratio);
		virtual void DeactivateResource();
		virtual std::shared_ptr<iResource> GetActiveResource();

		// Texture specifics
		bool Create2DTexture(const std::string& name, const eTextureType& type);
		bool CreateCubeTexture(const std::string& cubeMapName,
								const std::string& posX_fileName, const std::string& negX_fileName,
								const std::string& posY_fileName, const std::string& negY_fileName,
								const std::string& posZ_fileName, const std::string& negZ_fileName);

		std::shared_ptr<sSamplerInfo> GetSampler(const eTextureType& textureType);
		std::shared_ptr<sSamplerInfo> GetSampler(const std::string& textureName);
	private:
		TextureManager();

		void m_CreateSamplers();

		std::map<GLuint /* Sampler ID from OpenGL*/, std::shared_ptr<sSamplerInfo>> m_mapIdSampler;

		std::map<std::string, std::shared_ptr<iResource>> m_mapTextures;
		std::vector<std::shared_ptr<iResource>> m_vecTextures;

		std::shared_ptr<iResource> m_defaultTexture;

		// Texture binded at the moment
		std::string m_currTexture;
		std::string m_basePath;

		uint m_nextColorSamplerId = 0;
	};
}