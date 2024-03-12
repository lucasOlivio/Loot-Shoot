#pragma once

#include "Engine/ECS/Base.h"
#include "Engine/ThreadSafe.h"

#include "Engine/Gameplay/GameplayProperties.h"

namespace MyEngine
{
	struct PlayerComponent : public ThreadSafe
	{
		float speed;
	};

	struct SteeringBehaviorComponent : public ThreadSafe
	{
		eSteeringTypes steeringType;
		Entity targetId;

		float speed;
		float maxDistance;
		float futureTime;
	};
}
