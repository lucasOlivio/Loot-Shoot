#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

namespace MyEngine
{
	struct ParticleProps
	{
		size_t textureIndex;

		Entity entityId; // Entity that emitted this particle

		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 rotationSpeed; // In degrees

		glm::vec3 position;
		glm::quat orientation;
		float scale;

		float initialLifeTime;
		float lifetime;
		float alpha = 1.0f;
	};

	struct EmitterProps
	{
		std::vector<std::string> textures;
		std::vector<size_t> texturesIndex;

		// Offset from emitter current position
		glm::vec3 posMin;
		glm::vec3 posMax;

		glm::quat oriMin;
		glm::quat oriMax;

		// In degrees
		glm::vec3 rotMin;
		glm::vec3 rotMax;

		glm::vec3 constForce;

		glm::vec3 velMin;
		glm::vec3 velMax;

		float scaMin;
		float scaMax;

		float minLifeTime;
		float maxLifeTime;
	};
}
