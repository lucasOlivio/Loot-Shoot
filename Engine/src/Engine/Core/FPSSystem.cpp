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
		pFrame->frameCountUpdate = 0;
		pFrame->fpsTimerUpdate = 0;
		pFrame->frameCountRender = 0;
		pFrame->fpsTimerRender = 0;
		pFrame->UnlockWrite();
	}

	void FPSSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<FrameCounterComponent> pFrame = CoreLocator::GetFrameCounter();

		pFrame->LockWrite();
		pFrame->frameCountUpdate++;
		pFrame->fpsTimerUpdate += deltaTime;

		// Update Frame every second in the title
		if (pFrame->fpsTimerUpdate >= 1.0f)
		{
			pFrame->fpsUpdate = (float)(pFrame->frameCountUpdate) / pFrame->fpsTimerUpdate;
			pFrame->frameCountUpdate = 0;
			pFrame->fpsTimerUpdate = 0.0f;
		}
		pFrame->UnlockWrite();
	}

	void FPSSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<FrameCounterComponent> pFrame = CoreLocator::GetFrameCounter();

		float currentTime = static_cast<float>(glfwGetTime());
		pFrame->LockWrite();
		pFrame->fpsTimerRender += currentTime - pFrame->lastTimeRender;
		pFrame->lastTimeRender = currentTime;
		pFrame->frameCountRender++;

		// Render Frame every second in the title
		if (pFrame->fpsTimerRender >= 1.0f)
		{
			pFrame->fpsRender = (float)(pFrame->frameCountRender) / pFrame->fpsTimerRender;
			pFrame->frameCountRender = 0;
			pFrame->fpsTimerRender = 0.0f;
		}
		pFrame->UnlockWrite();
	}

	void FPSSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void FPSSystem::Shutdown()
	{
	}
}
