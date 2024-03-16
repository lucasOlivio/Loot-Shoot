#pragma once

#include "Engine/Core/types.h"
#include "Engine/Core/Resources/iResourceManager.h"

#include "Engine/Graphics/opengl.h"

namespace MyEngine
{
	enum eTextureType
	{
		COLOR = 0,
		TRANSPARENCY = 4,
		HEIGHTMAP = 8,
		NORMAL = 12,
		DISCARD = 16,
		CUBE = 20,
		SPECULAR = 24
	};

	// Names of uniforms for texture binding
	struct sSamplerInfo
	{
		sSamplerInfo(std::string _name, std::string _ratio, std::string _toggle, GLuint _samplerId)
			: name(_name), ratio(_ratio), toggle(_toggle), samplerId(_samplerId) {}

		std::string name;
		std::string ratio;
		std::string toggle;
		GLuint samplerId;
	};

	struct s24BitPixel
	{
		uchar redPixel = 0;
		uchar greenPixel = 0;
		uchar bluePixel = 0;
	};

	struct sTextureInfo : public iResource
	{
		virtual ~sTextureInfo()
		{
			ClearBMP();
		}

		std::string extension;
		const void* pPixels;
		GLuint textNumber;
		eTextureType type;

		ulong numRows;
		ulong numCols;

		ulong originalHeight;
		ulong originalWidth;
		ushort reserved1;
		ushort reserved2;
		ulong fileSize;
		ulong offsetInBits;
		ulong headerSize;

		ushort numberOfPlanes;
		ushort bitPerPixel;	// 1, 4, 8, 15, 24, 32, or 64
		ulong compressionMode;	// 0 if NOT compressed (i.e. 24 bit bitmap)
		ulong imageSizeInBytes;	// Total bytes in image - may be zero
		ulong pixelsPerMeterX;
		ulong pixelsPerMeterY;
		ulong numberOfLookUpTableEntries;	// Could be 0 for 24 bit format
		ulong numberOfImportantColours;	// 0 if all colours are important

		void ClearBMP()
		{
			// PNG lib handles the png files buffer
			if (extension == "png")
			{
				return;
			}

			delete[] this->pPixels;
			this->pPixels = 0;
		}
	};
}
