#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Should be the only one to change the transform component data
	class RotationSystem : public iSystem
	{
	public:
		RotationSystem() = default;
		virtual ~RotationSystem() { };

		virtual std::string SystemName() { return "RotationSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
