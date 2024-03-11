#pragma once

#include "Engine/ECS/BaseComponent.h"

#include "Engine/Physics/PhysicsProperties.h"

#include <glm/glm.hpp>

namespace MyEngine
{
	struct MovementComponent : public BaseComponent
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float drag;
		float maxSpeed;
	};

	struct RotationComponent : public BaseComponent
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float drag;
		float maxSpeed;
	};

	struct RigidBodyComponent : public BaseComponent
	{
		eBody bodyType;

		float radius;
	};
}
