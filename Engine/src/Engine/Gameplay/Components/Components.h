#pragma once

#include "Engine/ECS/Base.h"
#include "Engine/ECS/BaseComponent.h"

#include "Engine/Gameplay/GameplayProperties.h"

namespace MyEngine
{
	struct PlayerComponent : public BaseComponent
	{
		float speed;
	};

	struct SteeringBehaviorComponent : public BaseComponent
	{
		eSteeringTypes steeringType;
		Entity targetId;

		float speed;
		float maxDistance;
		float futureTime;
	};
}
