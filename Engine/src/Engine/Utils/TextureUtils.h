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
		// Updated: December 2010 for ATI cards, too!
		static bool CreateTextureFromBMPFile(std::string textureName, std::string fileNameFullPath, 
												bool bGenerateMIPMap, std::shared_ptr<sTextureInfo> pTextureOut);

		static bool CreateCubeTextureFromBMPFiles(std::string cubeMapName,
												std::string posX_fileName, std::string negX_fileName,
												std::string posY_fileName, std::string negY_fileName,
												std::string posZ_fileName, std::string negZ_fileName,
												bool bIsSeamless, std::shared_ptr<sTextureInfo> pTextureOut);

		static bool LoadBMP(std::string fileName, std::shared_ptr<sTextureInfo> pTextureOut);		// Faster loader
	};
}
