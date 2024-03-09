#pragma once

#include "Engine/ECS/Scene/Scene.h"
#include "Engine/ECS/System/iSystem.h"
#include "Engine/ECS/Scene/iSceneManager.h"

#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/CollisionEvent.h"
#include "Engine/Events/InputEvent.h"
#include "Engine/Events/SceneEvent.h"
#include "Engine/Events/GameStateEvent.h"
#include "Engine/Events/AnimationEvent.h"
#include "Engine/Events/iEventBus.h"

#include "Engine/Graphics/Renderer/iRendererManager.h"
#include "Engine/Graphics/Shaders/iShaderManager.h"
#include "Engine/Graphics/Textures/iTextureManager.h"
#include "Engine/Graphics/Particles/iParticleManager.h"

#include <map>

namespace MyEngine
{
	// App should inherit from this class to setup and run everything needed
	class Engine
	{
	public:
		Engine();
		virtual ~Engine();

		float GetDeltaTime();

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
		virtual void Run(std::string initialSceneName, bool startSimulation = true);

		virtual void Update();

		virtual void Render();

		virtual void Shutdown();

		virtual void LoadConfigurations();

		// Initializes systems with scene
		virtual void StartSystems(std::shared_ptr<Scene> pScene);

		// Finishes systems for scene
		virtual void EndSystems(std::shared_ptr<Scene> pScene);

		virtual void ShutdownSystems();

		virtual void OnSceneChange(const SceneChangeEvent& event);

		virtual void OnWindowClose(const WindowCloseEvent& event);

	protected:
		std::vector<std::shared_ptr<iSystem>> m_vecSystems;

		std::shared_ptr<Scene> m_currentScene = nullptr;

		// Events
		std::shared_ptr<iEventBus<eWindowEvents, WindowCloseEvent>> m_ebWindow;
		std::shared_ptr<iEventBus<eCollisionEvents, RigidBodyCollisionEvent>> m_ebRigidCollision;
		std::shared_ptr<iEventBus<eCollisionEvents, SoftBodyCollisionEvent>> m_ebSoftCollision;
		std::shared_ptr<iEventBus<eInputEvents, KeyboardEvent>> m_ebKeyboard;
		std::shared_ptr<iEventBus<eInputEvents, MouseEvent>> m_ebMouse;
		std::shared_ptr<iEventBus<eSceneEvents, SceneChangeEvent>> m_ebSceneChange;
		std::shared_ptr<iEventBus<eGameStateEvents, GameStartedEvent>> m_ebStartedState;
		std::shared_ptr<iEventBus<eGameStateEvents, GameRunningEvent>> m_ebRunningState;
		std::shared_ptr<iEventBus<eGameStateEvents, GameStoppedEvent>> m_ebStoppedState;
		std::shared_ptr<iEventBus<eGameStateEvents, GameOverEvent>> m_ebGameOverState;
		std::shared_ptr<iEventBus<eAnimationEvents, PositionKeyFrameEvent>> m_ebPosKeyFrame;
		std::shared_ptr<iEventBus<eAnimationEvents, RotationKeyFrameEvent>> m_ebRotKeyFrame;
		std::shared_ptr<iEventBus<eAnimationEvents, ScaleKeyFrameEvent>> m_ebScaleKeyFrame;

		// Resources managers
		std::shared_ptr<iShaderManager> m_shaderManager;
		std::shared_ptr<iTextureManager> m_textureManager;
		std::shared_ptr<iRendererManager> m_rendererManager;
		std::shared_ptr<iSceneManager> m_sceneManager;
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
	};
}
