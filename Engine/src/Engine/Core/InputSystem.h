#pragma once

#include "Engine/ECS/System/iSystem.h"
#include "Engine/Core/InputProperties.h"

namespace MyEngine
{
	// Manages GLFW input related events and keeps input component updated
	class InputSystem : public iSystem
	{
	public:
		InputSystem() = default;
		virtual ~InputSystem() { };

		virtual std::string SystemName() { return "InputSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		static void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

		static void MouseButtonCallback(GLFWwindow* window, int mouse, int action, int mods);

		static void MousePositionCallback(GLFWwindow* window, double xpos, double ypos);

	private:
		static void m_TriggerKeyEvent(const sKeyData& collData);

		static void m_TriggerMouseEvent(const sMouseData& collData);
	};
}
