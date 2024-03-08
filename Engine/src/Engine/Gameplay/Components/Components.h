#pragma once

#include "Engine/ECS/Base.h"
#include "Engine/ECS/iComponent.h"

#include "Engine/Gameplay/GameplayProperties.h"

namespace MyEngine
{
	struct PlayerComponent : public iComponent
	{
		float speed;
	};

	struct SteeringBehaviorComponent : public iComponent
	{
		eSteeringTypes steeringType;
		Entity targetId;

		float speed;
		float maxDistance;
		float futureTime;
	};
}
