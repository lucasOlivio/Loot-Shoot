#include "pch.h"

#include "StateSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Events/EventsFacade.h"

namespace MyEngine
{
	void StateSystem::Init()
	{
	}

	void StateSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void StateSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<GameStateComponent> pState = CoreLocator::GetGameState();
		pState->LockRead();
		if (pState->currState == pState->prevState)
		{
			pState->UnlockRead();
			return;
		}
		pState->UnlockRead();

		m_TriggerStateChange(pScene, pState->prevState, pState->currState);

		pState->LockWrite();
		pState->prevState = pState->currState;
		pState->UnlockWrite();
	}

	void StateSystem::Render(std::shared_ptr<Scene> pScene)
	{
	}

	void StateSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void StateSystem::Shutdown()
	{
	}

	void StateSystem::m_TriggerStateChange(std::shared_ptr<Scene> pScene, const eGameStates& oldState, const eGameStates& newState)
	{
		if (newState == eGameStates::STARTED)
		{
			GameStartedEvent stateEvent = GameStartedEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			PUBLISH_GAME_STARTED_EVENT(stateEvent);

			return;
		}
		else if (newState == eGameStates::STOPPED)
		{
			GameStoppedEvent stateEvent = GameStoppedEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			PUBLISH_GAME_STOPPED_EVENT(stateEvent);

			return;
		}
		else if (newState == eGameStates::RUNNING)
		{
			GameRunningEvent stateEvent = GameRunningEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			PUBLISH_GAME_RUNNING_EVENT(stateEvent);

			return;
		}
		else if (newState == eGameStates::GAMEOVER)
		{
			GameOverEvent stateEvent = GameOverEvent();
			stateEvent.prevState = oldState;
			stateEvent.pScene = pScene;
			PUBLISH_GAME_OVER_EVENT(stateEvent);

			return;
		}
	}
}
