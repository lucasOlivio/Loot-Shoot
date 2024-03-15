#include "pch.h"

#include "TextureUtils.h"

#include "BitOperations.h"

#include <png++/png.hpp>

namespace MyEngine
{
	bool TextureUtils::CreateTextureFromBMPFile(std::string textureName, std::string fileNameFullPath, bool bGenerateMIPMap, std::shared_ptr<sTextureInfo> pTextureOut)
	{
		// Clear any old openGL errors
		int clear = glGetError();

		// Pick a texture number...
		pTextureOut->textNumber = 0;
		glGenTextures(1, &(pTextureOut->textNumber));
		// Worked?
		if (CheckOpenGLError())
		{
			return false;
		}

		if (!LoadBMP(fileNameFullPath, pTextureOut))
		{
			return false;
		}

		// Good to go (valid texture ID and loaded bitmap...
		// Now set the texture...
		glBindTexture(GL_TEXTURE_2D, pTextureOut->textNumber);

		// In case texture is oddly aligned, set the client alignment to 1 byte (default is 4)
		GLint GL_UNPACK_ALIGNMENT_old = 0;
		glGetIntegerv(GL_UNPACK_ALIGNMENT, &GL_UNPACK_ALIGNMENT_old);
		// Set alignment to 1 byte
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexImage2D(GL_TEXTURE_2D,  // target (2D, 3D, etc.)
			0,					     // MIP map level 
			GL_RGBA,			     // internal format
			pTextureOut->numCols,	 // width (pixels)	
			pTextureOut->numRows,    // height (pixels)	
			0,					     // border (0 or 1)
			GL_RGB,			         // format of pixel data
			GL_UNSIGNED_BYTE,	     // type of pixel data
			pTextureOut->pPixels);  // pointer to data in memory

		if (CheckOpenGLError()) { return false; }

		// Put the pixel store aligment back to what it was
		glPixelStorei(GL_UNPACK_ALIGNMENT, GL_UNPACK_ALIGNMENT_old);

		pTextureOut->ClearBMP();

		if (bGenerateMIPMap)
		{
			glGenerateMipmap(GL_TEXTURE_2D);		// OpenGL 4.0
		}

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, /*GL_CLAMP*/ GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, /*GL_CLAMP*/ GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST /*GL_LINEAR*/);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		pTextureOut->name = textureName;

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	bool TextureUtils::CreateCubeTextureFromBMPFiles(std::string cubeMapName, std::string posX_fileName,
														std::string negX_fileName, std::string posY_fileName, 
														std::string negY_fileName, std::string posZ_fileName,	
														std::string negZ_fileName, bool bIsSeamless, 
														std::shared_ptr<sTextureInfo> pTextureOut)
	{
		GLenum discardError = glGetError();

		// Pick a texture number...
		glGenTextures(1, &(pTextureOut->textNumber));
		// Worked?
		if (CheckOpenGLError())
		{
			return false;
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, pTextureOut->textNumber);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE /*GL_REPEAT*/);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// IF YOU DON'T GENERATE MIP MAPS, then you can use LINEAR filtering
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		if (bIsSeamless)
		{
			glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
		}

		if (CheckOpenGLError()) { return false; }

		// Positive X image...
		// Assume all the images are the same size. If not, then it will screw up
		bool isLoaded = LoadBMP(posX_fileName, pTextureOut);
		assert(isLoaded);
		glTexStorage2D(GL_TEXTURE_CUBE_MAP,
						10,                    // Mipmap levels
						GL_RGBA8,	           // Internal format
						pTextureOut->numCols,  // width (pixels)
						pTextureOut->numRows); // height (pixels)

		if (CheckOpenGLError()) { return false; }

		// Positive X image...
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,
						0,                    // Level
						0, 0,                 // Offset
						pTextureOut->numCols, // width
						pTextureOut->numRows, // height
						GL_RGB,
						GL_UNSIGNED_BYTE,
						pTextureOut->pPixels);
		pTextureOut->ClearBMP();
		if (CheckOpenGLError()) { return false; }


		// Negative X image...
		isLoaded = LoadBMP(negX_fileName, pTextureOut);
		assert(isLoaded);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, 0, 0, pTextureOut->numCols, pTextureOut->numRows, GL_RGB, GL_UNSIGNED_BYTE, pTextureOut->pPixels);
		pTextureOut->ClearBMP();
		if (CheckOpenGLError()) { return false; }

		// Positive Y image...
		isLoaded = LoadBMP(posY_fileName, pTextureOut);
		assert(isLoaded);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, 0, 0, pTextureOut->numCols, pTextureOut->numRows, GL_RGB, GL_UNSIGNED_BYTE, pTextureOut->pPixels);
		pTextureOut->ClearBMP();
		if (CheckOpenGLError()) { return false; }

		// Negative Y image...
		isLoaded = LoadBMP(negY_fileName, pTextureOut);
		assert(isLoaded);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, 0, 0, pTextureOut->numCols, pTextureOut->numRows, GL_RGB, GL_UNSIGNED_BYTE, pTextureOut->pPixels);
		pTextureOut->ClearBMP();
		if (CheckOpenGLError()) { return false; }

		// Positive Z image...
		isLoaded = LoadBMP(posZ_fileName, pTextureOut);
		assert(isLoaded);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, 0, 0, pTextureOut->numCols, pTextureOut->numRows, GL_RGB, GL_UNSIGNED_BYTE, pTextureOut->pPixels);
		pTextureOut->ClearBMP();
		if (CheckOpenGLError()) { return false; }

		// Negative Z image...
		isLoaded = LoadBMP(negZ_fileName, pTextureOut);
		assert(isLoaded);
		glTexSubImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, 0, 0, pTextureOut->numCols, pTextureOut->numRows, GL_RGB, GL_UNSIGNED_BYTE, pTextureOut->pPixels);
		pTextureOut->ClearBMP();
		if (CheckOpenGLError()) { return false; }

		pTextureOut->name = cubeMapName;

		return true;
	}

	bool TextureUtils::LoadBMP(std::string fileName, std::shared_ptr<sTextureInfo> pTextureOut)
	{
		std::ifstream theFile;
		theFile.open(fileName.c_str(), std::ios_base::binary);
		assert(theFile);

		// Scan to the end to get the size
		theFile.seekg(0, std::ios::end);
		std::ios::pos_type fileSize = theFile.tellg();
		// Return to start
		theFile.seekg(0, std::ios::beg);

		char* pRawData = new char[static_cast<uint>(fileSize)];

		theFile.read(pRawData, fileSize);
		theFile.close();

		// Now go through and decode the BMP file.
		ulong curIndex = 0;
		char letter1 = BitOperations::ReadNextChar(pRawData, curIndex);
		char letter2 = BitOperations::ReadNextChar(pRawData, curIndex);
		if ((letter1 != 'B') && (letter2 != 'M'))
		{
			LOG_ERROR("Error loading texture: NOT A BMP FILE");
			return false;
		}

		// File is BMP
		pTextureOut->fileSize         = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->reserved1        = BitOperations::ReadNextUnsignedShort(pRawData, curIndex);
		pTextureOut->reserved2        = BitOperations::ReadNextUnsignedShort(pRawData, curIndex);
		pTextureOut->offsetInBits     = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->headerSize       = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->numCols          = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->numRows          = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->numberOfPlanes   = BitOperations::ReadNextUnsignedShort(pRawData, curIndex);
		pTextureOut->bitPerPixel      = BitOperations::ReadNextUnsignedShort(pRawData, curIndex);
		// Is is a 24 bit bitmap?
		if (pTextureOut->bitPerPixel != 24)
		{
			LOG_ERROR("Error loading texture: NOT A 24 BIT BMP");
			return false;
		}

		pTextureOut->compressionMode            = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->imageSizeInBytes           = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->pixelsPerMeterX            = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->pixelsPerMeterY            = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->numberOfLookUpTableEntries = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);
		pTextureOut->numberOfImportantColours   = BitOperations::ReadNextUnsignedLong(pRawData, curIndex);

		// 24 bits is 3 bytes per row
		long bytesPerRow = pTextureOut->numRows * 3;

		if ((pTextureOut->numCols % 4) != 0)
		{
			bytesPerRow = (bytesPerRow / 4) * 4 + 4;
		}

		long numberOfPaddingBytes = bytesPerRow - 3 * pTextureOut->numCols;

		// Allocate enough space...
		pTextureOut->pPixels = new s24BitPixel[pTextureOut->numCols * pTextureOut->numRows];

		// Did we run out of memory?
		if (!pTextureOut->pPixels)
		{
			LOG_ERROR("Error loading texture: NOT ENOUGH MEMORY FOR BITMAP");
			return false;
		}

		// Read the bitmap into memory...
		uint totalBytesInFile = bytesPerRow * pTextureOut->numCols;
		uint pixelCount = 0;

		for (uint row = 0; row < pTextureOut->numRows; row++)
		{
			for (uint col = 0; col < pTextureOut->numCols; col++)
			{
				char theblue  = BitOperations::ReadNextChar(pRawData, curIndex);
				char thegreen = BitOperations::ReadNextChar(pRawData, curIndex);
				char thered   = BitOperations::ReadNextChar(pRawData, curIndex);

				pTextureOut->pPixels[pixelCount].redPixel = static_cast<uchar>(thered);
				pTextureOut->pPixels[pixelCount].greenPixel = static_cast<uchar>(thegreen);
				pTextureOut->pPixels[pixelCount].bluePixel = static_cast<uchar>(theblue);

				pixelCount++;
			}

			// We are at the end of the row and there will 
			//	still be bytes padding up to a multiple of 4...
			// Read and discard these...
			for (int discardedBytes = 0; discardedBytes < numberOfPaddingBytes; discardedBytes++)
			{
				char tempChar = BitOperations::ReadNextChar(pRawData, curIndex);
			}
		}

		delete pRawData;

		return true;
	}
}
