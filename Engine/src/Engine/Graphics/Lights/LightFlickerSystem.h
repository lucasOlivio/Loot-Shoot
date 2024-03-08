#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	class LightFlickerSystem : public iSystem
	{
	public:
		LightFlickerSystem() = default;
		virtual ~LightFlickerSystem() { };

		virtual std::string SystemName() { return "LightFlickerSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
