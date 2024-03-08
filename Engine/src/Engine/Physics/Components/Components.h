#pragma once

#include "Engine/ECS/iComponent.h"

#include "Engine/Physics/PhysicsProperties.h"

#include <glm/glm.hpp>

namespace MyEngine
{
	struct MovementComponent : public iComponent
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float drag;
		float maxSpeed;
	};

	struct RotationComponent : public iComponent
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float drag;
		float maxSpeed;
	};

	struct RigidBodyComponent : public iComponent
	{
		eBody bodyType;

		float radius;
	};
}
