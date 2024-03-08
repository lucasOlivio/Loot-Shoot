#pragma once

#include "Engine/ECS/System/iSystem.h"

#include "Engine/Events/InputEvent.h"

namespace MyEngine
{
	// All player input handling
	class PlayerControllerSystem : public iSystem
	{
	public:
		PlayerControllerSystem() = default;
		virtual ~PlayerControllerSystem() { };

		virtual std::string SystemName() { return "PlayerControllerSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		static void InputTriggered(const KeyboardEvent& event);

	private:
		void m_InitiateMouseCapture();

		void m_StopMouseCapture();
	};
}
