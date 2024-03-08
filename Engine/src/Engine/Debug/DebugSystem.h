#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Initializes debug objects
	class DebugSystem : public iSystem
	{
	public:
		DebugSystem() = default;
		virtual ~DebugSystem() { };

		virtual std::string SystemName() { return "DebugSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
