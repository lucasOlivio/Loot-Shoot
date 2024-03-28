#include "pch.h"

#include "InputSystem.h"

#include "Engine/Graphics/Components/GraphicsLocator.h"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Events/EventsFacade.h"

namespace MyEngine
{
	void InputSystem::Init()
	{
		// TODO: Remove this coupling, this creates the need to the input system 
		// be registered after the window system
		std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();
		if (!pWindow->pGLFWWindow)
		{
			LOG_WARNING("GLFW Window not initialized, no input callbacks configured.");
			return;
		}

		pWindow->LockRead();
		glfwSetKeyCallback(pWindow->pGLFWWindow, InputSystem::KeyCallback);
		glfwSetMouseButtonCallback(pWindow->pGLFWWindow, InputSystem::MouseButtonCallback);
		glfwSetCursorPosCallback(pWindow->pGLFWWindow, InputSystem::MousePositionCallback);
		pWindow->UnlockRead();

		return;
	}

	void InputSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void InputSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
	}

	void InputSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
	{
	}

	void InputSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void InputSystem::Shutdown()
	{
	}

	void InputSystem::KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		// Update input component to keep track of pressed keys
		std::shared_ptr<KeyInputComponent> pKeyInput = CoreLocator::GetKeyInput();

		sKeyData keyData = sKeyData();
		keyData.key = (eKeyCodes)key;
		keyData.action = (eInputActions)action;
		keyData.mod = (eKeyMods)mods;

		pKeyInput->LockWrite();
		if (keyData.action == eInputActions::KEY_RELEASE)
		{
			pKeyInput->key[key] = false;
		}
		else
		{
			pKeyInput->key[key] = true;
		}
		pKeyInput->UnlockWrite();

		// Push keyboard event
		m_TriggerKeyEvent(keyData);

		return;
	}

	void InputSystem::MouseButtonCallback(GLFWwindow* window, int mouse, int action, int mods)
	{
		// Update input component to keep track of pressed mouse buttons
		std::shared_ptr<MouseInputComponent> pMouseInput = CoreLocator::GetMouseInput();

		sMouseData mouseData = sMouseData();
		mouseData.button = (eMouseCodes)mouse;
		mouseData.action = (eInputActions)action;
		mouseData.mod = (eKeyMods)mods;

		pMouseInput->LockWrite();
		if (mouseData.action == eInputActions::KEY_RELEASE)
		{
			pMouseInput->button[mouse] = false;
		}
		else
		{
			pMouseInput->button[mouse] = true;
		}
		pMouseInput->UnlockWrite();

		// Push mouseboard event
		m_TriggerMouseEvent(mouseData);

		return;
	}

	void InputSystem::MousePositionCallback(GLFWwindow* window, double xpos, double ypos)
	{
		// Update input component to keep track of mouse position
		std::shared_ptr<MouseInputComponent> pMouseInput = CoreLocator::GetMouseInput();

		pMouseInput->LockWrite();
		pMouseInput->lastPosX = pMouseInput->posX;
		pMouseInput->lastPosY = pMouseInput->posY;

		pMouseInput->posX = (float)xpos;
		pMouseInput->posY = (float)ypos;
		pMouseInput->UnlockWrite();

		return;
	}

	void InputSystem::m_TriggerKeyEvent(const sKeyData& collData)
	{
		KeyboardEvent keyEvent = KeyboardEvent();
		keyEvent.keyData = collData;
		PUBLISH_KEYBOARD_EVENT(keyEvent);

		return;
	}

	void InputSystem::m_TriggerMouseEvent(const sMouseData& collData)
	{
		MouseEvent mouseEvent = MouseEvent();
		mouseEvent.mouseData = collData;
		PUBLISH_MOUSE_EVENT(mouseEvent);

		return;
	}
}
