#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	// Should be the only one to change the transform component data
	class RotationSystem : public EntitySystem
	{
	public:
		RotationSystem() = default;
		virtual ~RotationSystem() { };

		virtual std::string SystemName() { return "RotationSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);
	};
}
