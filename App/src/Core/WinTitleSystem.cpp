#include "pch.h"

#include "WinTitleSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Graphics/Components/GraphicsLocator.h"

namespace MyEngine
{
    void WinTitleSystem::Init()
    {
    }

    void WinTitleSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void WinTitleSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<FrameCounterComponent> pFPS = CoreLocator::GetFrameCounter();
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        pFPS->LockRead();
        int fps = static_cast<int>(pFPS->fps);
        pFPS->UnlockRead();

        pWindow->name = "UPDATE FPS: " + std::to_string(fps);
    }

    void WinTitleSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void WinTitleSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void WinTitleSystem::Shutdown()
    {
    }
}
