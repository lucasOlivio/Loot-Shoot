#include "pch.h"

#include "FPSSystem.h"

#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"
#include "Engine/ECS/SingletonComponents/CoreLocator.h"

namespace MyEngine
{
	void FPSSystem::Init()
	{
	}

	void FPSSystem::Start(std::shared_ptr<Scene> pScene)
	{
		std::shared_ptr<FrameCounterComponent> pFrame = CoreLocator::GetFrameCounter();

		pFrame->frameCount = 0;

		pFrame->fpsTimer = 0;
	}

	void FPSSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<FrameCounterComponent> pFrame = CoreLocator::GetFrameCounter();

		pFrame->frameCount++;

		pFrame->fpsTimer += deltaTime;

		// Update Frame every second in the title
		if (pFrame->fpsTimer >= 1.0f)
		{
			pFrame->fps = (float)(pFrame->frameCount) / pFrame->fpsTimer;

			// Update window name
			std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();
			pWindow->name = std::to_string(pFrame->fps);

			pFrame->frameCount = 0;
			pFrame->fpsTimer = 0.0f;
		}
	}

	void FPSSystem::Render(std::shared_ptr<Scene> pScene)
	{
	}

	void FPSSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void FPSSystem::Shutdown()
	{
	}
}
