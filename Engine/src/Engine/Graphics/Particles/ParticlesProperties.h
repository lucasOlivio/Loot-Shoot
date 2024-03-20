#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"
#include "Engine/ThreadSafe.h"

namespace MyEngine
{
	struct ParticleProps : public ThreadSafe
	{
		Entity entityId; // Entity that emitted this particle

		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 rotationSpeed; // In degrees

		glm::vec4 colorChange;

		float initialLifeTime;

		glm::vec3 position = glm::vec3(1.0f);
		glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
		float scale = 1.0f;

		// Buffer attributes
		glm::vec4 color = glm::vec4(1.0f);
		float alpha = 1.0f;
		float lifetime = 0.0f;
		glm::mat4 transform = glm::mat4(1.0f);
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
