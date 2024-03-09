#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

namespace MyEngine
{
	struct ParticleProps
	{
		std::vector<uint> FBOIDs = { 0 };

		std::shared_ptr<sMeshInfo> pMesh;
		std::string material;

		glm::vec3 defaultColor;
		glm::vec3 colorChange;

		Entity entityId; // Entity that emitted this particle

		glm::vec3 velocity;
		glm::vec3 acceleration;
		glm::vec3 rotationSpeed; // In degrees

		glm::vec3 position;
		glm::quat orientation;
		float scale;

		float lifetime;
	};

	struct EmitterProps
	{
		std::vector<uint> FBOIDs = { 0 };

		std::string meshName;
		std::string material;

		// The particle changes color over time if no material selected
		glm::vec3 colorInitial;
		glm::vec3 colorChange;

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
