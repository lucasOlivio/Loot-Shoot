#pragma once

#include "Engine/Core/StateProperties.h"

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Triggers state changes events when state component changes
	class StateSystem : public iSystem
	{
	public:
		StateSystem() = default;
		virtual ~StateSystem() { };

		virtual std::string SystemName() { return "StateSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	private:
		static void m_TriggerStateChange(std::shared_ptr<Scene> pScene, const eGameStates& oldState, const eGameStates& newState);
	};
}
