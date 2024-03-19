#pragma once

#include "Engine/Graphics/GraphicsProperties.h"

#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include <glm/mat4x4.hpp>

namespace MyEngine
{
	class GraphicsUtils
	{
	public:
		// Sets all needed uniforms and draw model using VAO and opengl
		static void DrawModel(const sRenderModelInfo& renderInfo, std::shared_ptr<ShaderManager> pShader);

		// Use instancing to render as much objects as needed updating the buffer data
		static void DrawInstanced(const uint& BUFFER_ID, const uint& VAO_ID,
								  const uint& numIndices/*, 
								  const std::vector<sRenderInfo>& renderInfo*/);
	};
}
