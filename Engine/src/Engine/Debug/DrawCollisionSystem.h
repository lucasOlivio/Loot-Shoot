#pragma once

#include "Engine/ECS/System/iSystem.h"
#include "Engine/Events/CollisionEvent.h"

namespace MyEngine
{
	// Draw collisions on collision event notification
	class DrawCollisionSystem : public iSystem
	{
	public:
		DrawCollisionSystem() = default;
		virtual ~DrawCollisionSystem() { };

		virtual std::string SystemName() { return "DrawCollisionSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
