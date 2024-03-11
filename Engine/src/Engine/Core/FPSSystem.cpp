#include "pch.h"

#include "FPSSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

namespace MyEngine
{
	void FPSSystem::Init()
	{
	}

	void FPSSystem::Start(std::shared_ptr<Scene> pScene)
	{
		std::shared_ptr<FrameCounterComponent> pFrame = CoreLocator::GetFrameCounter();

		pFrame->LockWrite();
		pFrame->frameCount = 0;
		pFrame->fpsTimer = 0;
		pFrame->UnlockWrite();
	}

	void FPSSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<FrameCounterComponent> pFrame = CoreLocator::GetFrameCounter();

		pFrame->LockWrite();
		pFrame->frameCount++;
		pFrame->fpsTimer += deltaTime;

		// Update Frame every second in the title
		if (pFrame->fpsTimer >= 1.0f)
		{
			pFrame->fps = (float)(pFrame->frameCount) / pFrame->fpsTimer;
			pFrame->frameCount = 0;
			pFrame->fpsTimer = 0.0f;
		}
		pFrame->UnlockWrite();
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
