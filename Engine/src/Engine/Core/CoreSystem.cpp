#include "pch.h"

#include "CoreSystem.h"

#include "Engine/Core/EngineLocator.h"

#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Events/EventBusLocator.hpp"

namespace MyEngine
{
    void CoreSystem::Init()
    {
        // Subscribe to enter collision event
        std::shared_ptr<iEventBus<eGameStateEvents, GameStartedEvent>> pStartedBus = EventBusLocator<eGameStateEvents, GameStartedEvent>::Get();
        std::shared_ptr<iEventBus<eGameStateEvents, GameRunningEvent>> pRunningBus = EventBusLocator<eGameStateEvents, GameRunningEvent>::Get();
        std::shared_ptr<iEventBus<eGameStateEvents, GameStoppedEvent>> pStoppedBus = EventBusLocator<eGameStateEvents, GameStoppedEvent>::Get();
        std::shared_ptr<iEventBus<eGameStateEvents, GameOverEvent>> pGameOverBus = EventBusLocator<eGameStateEvents, GameOverEvent>::Get();

        pStartedBus->Subscribe(eGameStateEvents::GAME_STARTED, OnStart);
        pRunningBus->Subscribe(eGameStateEvents::GAME_RUNNING, OnRunning);
        pStoppedBus->Subscribe(eGameStateEvents::GAME_STOPPED, OnStopped);
        pGameOverBus->Subscribe(eGameStateEvents::GAME_OVER, OnGameOver);

        // Main systems must start right away
        std::shared_ptr<Engine> pEngine = EngineLocator::Get();
        std::shared_ptr<GameStateComponent> pState = CoreLocator::GetGameState();
        for (std::string systemName : pState->mainSystems)
        {
            pEngine->AddSystem(systemName);
        }
    }

    void CoreSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void CoreSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void CoreSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void CoreSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void CoreSystem::Shutdown()
    {
        // Subscribe to enter collision event
        std::shared_ptr<iEventBus<eGameStateEvents, GameStartedEvent>> pStartedBus = EventBusLocator<eGameStateEvents, GameStartedEvent>::Get();
        std::shared_ptr<iEventBus<eGameStateEvents, GameRunningEvent>> pRunningBus = EventBusLocator<eGameStateEvents, GameRunningEvent>::Get();
        std::shared_ptr<iEventBus<eGameStateEvents, GameStoppedEvent>> pStoppedBus = EventBusLocator<eGameStateEvents, GameStoppedEvent>::Get();
        std::shared_ptr<iEventBus<eGameStateEvents, GameOverEvent>> pGameOverBus = EventBusLocator<eGameStateEvents, GameOverEvent>::Get();

        pStartedBus->Unsubscribe(eGameStateEvents::GAME_STARTED, OnStart);
        pRunningBus->Unsubscribe(eGameStateEvents::GAME_RUNNING, OnRunning);
        pStoppedBus->Unsubscribe(eGameStateEvents::GAME_STOPPED, OnStopped);
        pGameOverBus->Unsubscribe(eGameStateEvents::GAME_OVER, OnGameOver);
    }

    void CoreSystem::OnStart(const GameStartedEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::STARTED, event.pScene);
    }

    void CoreSystem::OnRunning(const GameRunningEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::RUNNING, event.pScene);
    }

    void CoreSystem::OnStopped(const GameStoppedEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::STOPPED, event.pScene);
    }

    void CoreSystem::OnGameOver(const GameOverEvent& event)
    {
        ChangeStateSystems(event.prevState, eGameStates::GAMEOVER, event.pScene);
    }

    void CoreSystem::ChangeStateSystems(eGameStates prevstate, eGameStates currstate, std::shared_ptr<Scene> pScene)
    {
        using namesIt = std::vector<std::string>::iterator;

        std::shared_ptr<Engine> pEngine = EngineLocator::Get();
        std::shared_ptr<GameStateComponent> pState = CoreLocator::GetGameState();

        // Remove only systems that are not in the current state
        for (std::string systemName : pState->mapStateSystems[prevstate])
        {
            namesIt it = std::find(pState->mapStateSystems[currstate].begin(), pState->mapStateSystems[currstate].end(), systemName);

            if (it == pState->mapStateSystems[currstate].end())
            {
                pEngine->RemoveSystem(systemName, pScene);
            }
        }

        // Add only systems that are not in the previous state
        for (std::string systemName : pState->mapStateSystems[currstate])
        {
            pEngine->AddSystem(systemName, pScene);
        }
    }
}
