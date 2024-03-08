#pragma once

#include "Engine/ECS/Base.h"
#include "Engine/ECS/iComponent.h"

#include "Engine/Graphics/Textures/TextureProperties.h"
#include "Engine/Graphics/GraphicsProperties.h"
#include "Engine/Graphics/Animations/AnimationProperties.h"
#include "Engine/Graphics/Particles/ParticlesProperties.h"


#include "Engine/Gameplay/GameplayProperties.h"

#include <glm/gtc/quaternion.hpp>
#include <set>

namespace MyEngine
{
	// TODO: Player Gameplay would be better separated from engine, using scripts instead of components
	// Gameplay
	// -------------------------------------------------------------
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
