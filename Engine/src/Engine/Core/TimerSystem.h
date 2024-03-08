#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Keeps counting the running time in ms and seconds for the whole system
	class TimerSystem : public iSystem
	{
	public:
		TimerSystem() = default;
		virtual ~TimerSystem() { };

		virtual std::string SystemName() { return "TimerSystem"; };

		virtual void Init();

		virtual void Start(Scene* pScene);

		virtual void Update(Scene* pScene, float deltaTime);

		virtual void Render(Scene* pScene);

		virtual void End(Scene* pScene);

		virtual void Shutdown();
	};
}