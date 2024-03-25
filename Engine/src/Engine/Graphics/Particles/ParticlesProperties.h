#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

#include "Engine/Graphics/opengl.h"

#include "Engine/ThreadSafe.h"

namespace MyEngine
{
	struct ParticleProps : public ThreadSafe
	{
		Entity entityId; // Entity that emitted this particle
		GLuint numTexture;

		glm::vec3 velocity;
		glm::vec3 acceleration;

		float initialLifeTime;

		// Buffer attributes
		float alpha = 1.0f;
		float lifetime = 0.0f;
		glm::mat4 transform = glm::mat4(1.0f);
	};

	struct EmitterProps
	{
		// Offset from emitter current position
		glm::vec3 posMin;
		glm::vec3 posMax;

		glm::vec3 constForce;

		glm::vec3 velMin;
		glm::vec3 velMax;

		float scaMin;
		float scaMax;

		float minLifeTime;
		float maxLifeTime;

		std::vector<std::string> textures;
		std::vector<GLuint> numTextures;
	};
}
