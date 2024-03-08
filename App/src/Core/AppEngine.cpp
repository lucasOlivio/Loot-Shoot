#include "AppEngine.h"

#include "Engine/ECS/SingletonComponents/CoreLocator.h"
#include "Engine/ECS/System/SystemBuilder.h"

// Custom app systems
#include "Gameplay/PlayerControllerSystem.h"

// Here we use to include all we need for this specific app details to work
namespace MyEngine
{
	Application::Application()
	{
		Init();
	}
	
	Application::~Application()
	{
	}

	// TODO: For now duplicated from App we just need editor to create the scenes,
	// but later this should all come separated from config and better system management
	void Application::Init()
	{
		Engine::Init();

		// Register custom app systems
		// SystemBuilder::RegisterSystem("ExampleCustomSystem", []() { return new ExampleCustomSystem; });
		SystemBuilder::RegisterSystem("PlayerControllerSystem", []() { return new PlayerControllerSystem(); });

		std::shared_ptr<GameStateComponent> pStates = CoreLocator::GetGameState();

		pStates->mainSystems.push_back("FPSSystem");
		pStates->mainSystems.push_back("TransformParentSystem");
		pStates->mainSystems.push_back("WindowSystem");
		pStates->mainSystems.push_back("InputSystem");
		pStates->mainSystems.push_back("BaseUISystem"); // Has to come after inputsystem, to init imgui after we register out input callback
		// Graphics
		pStates->mainSystems.push_back("ShaderSystem");
		pStates->mainSystems.push_back("ModelRenderSystem");
		pStates->mainSystems.push_back("RenderSystem");
		pStates->mainSystems.push_back("LightSystem");
		pStates->mainSystems.push_back("AnimationSystem");

		// TODO: This could come from a config file
		// TODO: Could this be categorized to avoid having to put all in the config?
		pStates->mapStateSystems[eGameStates::NOT_STARTED] = {};

		pStates->mapStateSystems[eGameStates::STARTED] = {};

		pStates->mapStateSystems[eGameStates::RUNNING] = {
			// Graphics
			"AnimationPlayerSystem",
			// Physics
			"MovementSystem",
			"RotationSystem",
			"GridBroadPhaseSystem",
			"CollisionSystem",
			// Gameplay
			//"PlayerControllerSystem",
			"FlyingCameraSystem"

			// Debug
			,"DebugSystem",
			"DrawGridSystem",
			"DrawCollisionSystem"

		};

		pStates->mapStateSystems[eGameStates::STOPPED] = {
			// Physics
			"GridBroadPhaseSystem"
			#ifdef DEBUG
			// Debug
			,"DebugSystem",
			"DrawGridSystem",
			"DrawCollisionSystem"
			#endif
		};

		pStates->mapStateSystems[eGameStates::GAMEOVER] = {};
	}
}

