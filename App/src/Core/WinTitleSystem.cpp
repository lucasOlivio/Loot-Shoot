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
        int fpsUpdate = static_cast<int>(pFPS->fpsUpdate);
        int fpsRender = static_cast<int>(pFPS->fpsRender);
        pFPS->UnlockRead();

        std::string newTitle = "UPDATE FPS: " + std::to_string(fpsUpdate) + " | RENDER FPS: " + std::to_string(fpsRender);

        pWindow->LockWrite();
        pWindow->name = newTitle;
        pWindow->UnlockWrite();
    }

    void WinTitleSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void WinTitleSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void WinTitleSystem::Shutdown()
    {
    }
}
