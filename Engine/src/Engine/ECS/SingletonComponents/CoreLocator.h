#pragma once

#include "Engine/ECS/SingletonComponents/Components.h"

namespace MyEngine
{
	class CoreLocator
	{
	public:
		static std::shared_ptr<ConfigPathComponent> GetConfigPath();
		static std::shared_ptr<FrameCounterComponent> GetFrameCounter();
		static std::shared_ptr<KeyInputComponent> GetKeyInput();
		static std::shared_ptr<MouseInputComponent> GetMouseInput();
		static std::shared_ptr<GameStateComponent> GetGameState();

		static void SetConfigPath(std::shared_ptr<ConfigPathComponent> pConfigPath);
		static void SetFrameCounter(std::shared_ptr<FrameCounterComponent> pFrameCounter);
		static void SetKeyInput(std::shared_ptr<KeyInputComponent> pKeyInput);
		static void SetMouseInput(std::shared_ptr<MouseInputComponent> pMouseInput);
		static void SetGameState(std::shared_ptr<GameStateComponent> pState);

	private:
		static std::shared_ptr<ConfigPathComponent> m_pConfigPath;
		static std::shared_ptr<FrameCounterComponent> m_pFrameCounter;
		static std::shared_ptr<KeyInputComponent> m_pKeyInput;
		static std::shared_ptr<MouseInputComponent> m_pMouseInput;
		static std::shared_ptr<GameStateComponent> m_pState;
	};
}
