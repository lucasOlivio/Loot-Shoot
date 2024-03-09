#pragma once

#include "Engine/Core/Resources/iResourceManager.h"

#include <glm/glm.hpp>

namespace MyEngine
{
	struct sMaterialInfo : public iResource
	{
		std::string alphaTexture = "";
		std::string cubeTexture = "";
		std::string discardTexture = "";
		std::string specularTexture = "";
		std::string normalTexture = "";
		std::string heightMapTexture = "";
		std::vector<std::string> colorTextures = {};

		glm::vec3 offset = glm::vec3(0.0f); // fixed offset

		glm::vec3 currOffset = glm::vec3(0.0f);
		glm::vec3 offsetMove = glm::vec3(0.0f);

		glm::vec3 currOffsetHeightMap = glm::vec3(0.0f);
		glm::vec3 offsetHeightMap = glm::vec3(0.0f);

		glm::vec3 colorTexturesRatios = glm::vec3(0.0f);

		float heightScale = 0.0f;

		bool useHeightMap = false;
		bool useNormalTexture = false;
		bool useSpecularTexture = false;
		bool useDiscardTexture = false;
		bool useCubeTexture = false;
		bool isEmissive = false;
		bool useAlphaTexture = false;
	};
}
