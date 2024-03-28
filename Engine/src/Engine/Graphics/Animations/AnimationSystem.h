#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	// Handles all the transform movements based on the time and the key frames for the animation
	class AnimationSystem : public EntitySystem
	{
	public:
		AnimationSystem() = default;
		virtual ~AnimationSystem() { };

		virtual std::string SystemName() { return "AnimationSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);
	};
}
