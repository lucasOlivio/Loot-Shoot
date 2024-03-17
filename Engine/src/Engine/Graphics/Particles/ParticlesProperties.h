#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

namespace MyEngine
{
	struct ParticleProps
	{
		Entity entityId; // Entity that emitted this particle

		glm::vec4 color = glm::vec4(1.0f);
		glm::vec4 colorChange;

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
		// The particle changes color over time if no material selected
		glm::vec4 colorInitial;
		glm::vec4 colorChange;

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
