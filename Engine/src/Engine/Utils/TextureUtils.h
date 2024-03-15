#pragma once

#include "Engine/Graphics/opengl.h"

#include "Engine/Core/Resources/Textures/Texture.h"

#include <string>

namespace MyEngine
{
	class TextureUtils
	{
	public:
		// Returns true if able to load texture and store it
		static bool CreateTextureFromFile(const std::string& textureName, const std::string& fileNameFullPath, 
												bool bGenerateMIPMap, std::shared_ptr<sTextureInfo> pTextureOut);

		// NOTE: All files must have same extension
		static bool CreateCubeTextureFromFiles(const std::string& cubeMapName,
												const std::string& posX_fileName, const std::string& negX_fileName,
												const std::string& posY_fileName, const std::string& negY_fileName,
												const std::string& posZ_fileName, const std::string& negZ_fileName,
												bool bIsSeamless, std::shared_ptr<sTextureInfo> pTextureOut);

		static bool LoadBMP(const std::string& fileName, std::shared_ptr<sTextureInfo> pTextureOut);

		static bool LoadPNG(const std::string& fileName, std::shared_ptr<sTextureInfo> pTextureOut);
	};
}
