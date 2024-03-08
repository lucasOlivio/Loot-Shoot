#include "pch.h"

#include "CoreLocator.h"

namespace MyEngine
{
    std::shared_ptr<ConfigPathComponent> CoreLocator::m_pConfigPath(new ConfigPathComponent());
    std::shared_ptr<FrameCounterComponent> CoreLocator::m_pFrameCounter(new FrameCounterComponent());
    std::shared_ptr<KeyInputComponent> CoreLocator::m_pKeyInput(new KeyInputComponent());
    std::shared_ptr<MouseInputComponent> CoreLocator::m_pMouseInput(new MouseInputComponent());
    std::shared_ptr<GameStateComponent> CoreLocator::m_pState(new GameStateComponent());

    std::shared_ptr<ConfigPathComponent> CoreLocator::GetConfigPath()
    {
        return m_pConfigPath;
    }

    std::shared_ptr<FrameCounterComponent> CoreLocator::GetFrameCounter()
    {
        return m_pFrameCounter;
    }

    std::shared_ptr<KeyInputComponent> CoreLocator::GetKeyInput()
    {
        return m_pKeyInput;
    }

    std::shared_ptr<MouseInputComponent> CoreLocator::GetMouseInput()
    {
        return m_pMouseInput;
    }

    std::shared_ptr<GameStateComponent> CoreLocator::GetGameState()
    {
        return m_pState;
    }

    void CoreLocator::SetConfigPath(std::shared_ptr<ConfigPathComponent> pConfigPath)
    {
        m_pConfigPath = pConfigPath;
    }

    void CoreLocator::SetFrameCounter(std::shared_ptr<FrameCounterComponent> pFrameCounter)
    {
        m_pFrameCounter = pFrameCounter;
    }

    void CoreLocator::SetKeyInput(std::shared_ptr<KeyInputComponent> pKeyInput)
    {
        m_pKeyInput = pKeyInput;
    }

    void CoreLocator::SetMouseInput(std::shared_ptr<MouseInputComponent> pMouseInput)
    {
        m_pMouseInput = pMouseInput;
    }

    void CoreLocator::SetGameState(std::shared_ptr<GameStateComponent> pState)
    {
        m_pState = pState;
    }
}
