#pragma once

#include "Engine/Core/Threads/iThreadPool.h"

#include "Engine/ECS/Scene/Scene.h"
#include "Engine/ECS/System/iSystem.h"
#include "Engine/ECS/Scene/iSceneManager.h"

#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/SceneEvent.h"

#include "Engine/Graphics/Renderer/iRendererManager.h"
#include "Engine/Graphics/Particles/iParticleManager.h"

#include <map>
#include <Windows.h>

namespace MyEngine
{
	// App should inherit from this class to setup and run everything needed
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		// Systems that will manipulate components and handle the scene in some way,
		// the system is added and initialized
		virtual void AddSystem(std::string systemName);
		// Add and start system
		virtual void AddSystem(std::string systemName, std::shared_ptr<Scene> pScene);

		// Ends scene for system, shutdown and deletes pointer
		virtual void RemoveSystem(std::string systemName, std::shared_ptr<Scene> pScene);

		virtual std::shared_ptr<iSystem> GetSystem(std::string systemName);

		virtual void Init();

		// Run engine starting simulation state as running or stopped
		virtual void Run(bool startSimulation = true);

		virtual bool IsRunning();

		virtual void Update();

		virtual void Render();

		virtual void Shutdown();

		virtual void LoadConfigurations();

		// Initializes systems with scene
		virtual void StartSystems(std::shared_ptr<Scene> pScene);

		// Finishes systems for scene
		virtual void EndSystems(std::shared_ptr<Scene> pScene);

		virtual void ShutdownSystems();

		virtual void OnWindowClose(const WindowCloseEvent& event);

	protected:
		std::vector<std::shared_ptr<iSystem>> m_vecSystems;

		std::shared_ptr<Scene> m_pScene = nullptr;

		// Resources managers
		std::shared_ptr<iRendererManager> m_rendererManager;
		std::shared_ptr<iParticleManager> m_particleManager;

		float m_lastTime = 0.0f;
		std::vector<float> m_frameTimes;

		bool m_isRunning;

		// Any major clears needed to be done at end of frame (Ex: scene deleting, entity delete)
		virtual void m_ClearFrame();

		// Operations to execute before rendering
		virtual void m_BeginFrame();

		// Operations to execute after rendering
		virtual void m_EndFrame();

		// Updates the timer
		float m_GetDeltaTime();

		// Thread pool main service
		std::shared_ptr<iThreadPool> m_threadPool;

		// All systems update. Runs one thread per-system them wait for them all to finish
		static DWORD WINAPI m_Update(LPVOID lpParam);
	};
}
