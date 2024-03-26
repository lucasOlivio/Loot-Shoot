#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

#include "Engine/Graphics/opengl.h"

#include "Engine/ThreadSafe.h"

namespace MyEngine
{
	struct ParticleProps : public ThreadSafe
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float lifetime = 0.0f;
		glm::mat4 transform = glm::mat4(1.0f);
	};
}
