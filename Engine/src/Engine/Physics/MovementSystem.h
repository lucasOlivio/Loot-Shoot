#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	// Should be the only one to change the transform component data
	class MovementSystem : public EntitySystem
	{
	public:
		MovementSystem() = default;
		virtual ~MovementSystem() { };

		virtual std::string SystemName() { return "MovementSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);
	};
}
