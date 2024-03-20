#pragma once

#include "Engine/Core/Resources/iResourceManager.h"

#include <vector>
#include <map>

namespace MyEngine
{
	enum eShaderType
	{
		VERTEX_SHADER,
		FRAGMENT_SHADER,
		UNKNOWN
	};

	class Shader
	{
	public:
		Shader() {};
		~Shader() {};

		uint ID = 0;    // ID from OpenGL
		std::vector<std::string> vecSource;
		std::string fileName;

		eShaderType shaderType = eShaderType::UNKNOWN;
		bool isSourceMultiLine;
	};

	class ShaderProgram : public iResource
	{
	public:
		uint ID;	// ID from OpenGL

		std::map< std::string /*name of uniform variable*/,
					int /* uniform location ID */ >
					mapUniformToUL;
		std::map< std::string /*name of attribute variable*/,
					int /* attribute location ID */ >
					mapAttributeToAL;
	};
}
