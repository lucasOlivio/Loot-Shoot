#pragma once

#include "Engine/ThreadSafe.h"

#include "Engine/Physics/PhysicsProperties.h"

#include <glm/glm.hpp>

namespace MyEngine
{
	struct MovementComponent : public ThreadSafe
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float drag;
		float maxSpeed;
	};

	struct RotationComponent : public ThreadSafe
	{
		glm::vec3 velocity;
		glm::vec3 acceleration;

		float drag;
		float maxSpeed;
	};

	struct RigidBodyComponent : public ThreadSafe
	{
		eBody bodyType;

		float radius;
	};
}
