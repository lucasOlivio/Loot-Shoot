#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Manages imgui initialization and shutdown
	class BaseUISystem : public iSystem
	{
	public:
		BaseUISystem() = default;
		virtual ~BaseUISystem() { };

		virtual std::string SystemName() { return "BaseUISystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
