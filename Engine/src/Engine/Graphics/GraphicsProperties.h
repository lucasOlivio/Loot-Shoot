#pragma once

#include "Engine/Core/Shapes.hpp"
#include <string>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

namespace MyEngine
{
	struct sRenderParticleInfo
	{
		float alpha = 1.0f;
		glm::vec4 color = glm::vec4(1.0f);

		glm::mat4 matModel = glm::mat4(1.0f);
	};

	struct sRenderModelInfo
	{
		std::string materialName = "";
		
		glm::mat4 matModel = glm::mat4(1.0f);
		glm::vec4 defaultColor = glm::vec4(1.0f);
		glm::vec3 tileAxis = glm::vec3(1.0f);
		glm::vec3 tileOffset = glm::vec3(0.0f);

		int VAO_ID = 0;
		int numberOfIndices = 0;

		float distToCamera = 0.0f;

		bool useDefaultColor = false;
		bool useColorTexture = false;
	};
}
