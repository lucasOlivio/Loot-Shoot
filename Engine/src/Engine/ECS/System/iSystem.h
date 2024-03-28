#pragma once

#include "Engine/ECS/Base.h"
#include "Engine/ECS/Scene/Scene.h"

namespace MyEngine
{
	class iSystem
	{
	public:
		iSystem() = default;
		virtual ~iSystem() { };

		virtual std::string SystemName() = 0;

		// Initializes the system before scene loading
		virtual void Init() = 0;

		// Any initialization the system needs with the scene loaded
		virtual void Start(std::shared_ptr<Scene> pScene) = 0;

		// Simulation per frame
		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime) = 0;

		// Any rendering in the window
		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime) = 0;

		// Finished this specific scene simulation, not necessarily end of app
		virtual void End(std::shared_ptr<Scene> pScene) = 0;

		// End of app, cleaning of all
		virtual void Shutdown() = 0;
	};
}
