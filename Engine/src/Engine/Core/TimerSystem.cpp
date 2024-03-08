#include "pch.h"

#include "TimerSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

namespace MyEngine
{
	void TimerSystem::Init()
	{
	}

	void TimerSystem::Start(Scene* pScene)
	{
		std::shared_ptr<TimerComponent> pTimer = CoreLocator::GetTimer();

		pTimer->timeTrack = 0.0f;
		pTimer->miliseconds = 0;
		pTimer->seconds = 0;
	}

	void TimerSystem::Update(Scene* pScene, float deltaTime)
	{
		std::shared_ptr<TimerComponent> pTimer = CoreLocator::GetTimer();

		pTimer->timeTrack += deltaTime * 1000;

		// Precision timer for miliseconds
		if (pTimer->timeTrack > 1.0f)
		{
			int ms = static_cast<int>(pTimer->timeTrack);

			pTimer->miliseconds += ms;
			pTimer->timeTrack -= static_cast<float>(ms);
		}

		pTimer->seconds = pTimer->miliseconds / 1000;
	}

	void TimerSystem::Render(Scene* pScene)
	{
	}

	void TimerSystem::End(Scene* pScene)
	{
	}

	void TimerSystem::Shutdown()
	{
	}
}
