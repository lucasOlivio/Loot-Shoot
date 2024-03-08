#pragma once

#include "Engine/ECS/System/iSystem.h"
#include "Engine/ECS/Components.h"

#include "Engine/Core/StateProperties.h"

#include "Engine/Events/GameStateEvent.h"

namespace MyEngine
{
	// Adds and removes other systems based on game state
	class CoreSystem : public iSystem
	{
	public:
		CoreSystem() = default;
		virtual ~CoreSystem() { };

		virtual std::string SystemName() { return "CoreSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		static void OnStart(const GameStartedEvent& event);

		static void OnRunning(const GameRunningEvent& event);

		static void OnStopped(const GameStoppedEvent& event);

		static void OnGameOver(const GameOverEvent& event);

		static void ChangeStateSystems(eGameStates prevstate, eGameStates currstate, std::shared_ptr<Scene> pScene);

	};
}
