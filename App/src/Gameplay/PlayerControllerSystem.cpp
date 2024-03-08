#include "pch.h"

#include "PlayerControllerSystem.h"


#include "Engine/ECS/SingletonComponents/CoreLocator.h"
#include "Engine/ECS/SingletonComponents/GraphicsLocator.h"
#include "Engine/ECS/Scene/SceneView.hpp"
#include "Engine/ECS/Components.h"

#include "Engine/Events/InputEvent.h"
#include "Engine/Events/WindowEvents.h"
#include "Engine/Events/EventBusLocator.hpp"

#include "Engine/Core/InputProperties.h"

#include "Engine/Utils/InputUtils.h"

namespace MyEngine
{
	void PlayerControllerSystem::Init()
	{
        // Subscribe to keyboard event
        std::shared_ptr<iEventBus<eInputEvents, KeyboardEvent>> pEventBus = EventBusLocator<eInputEvents, KeyboardEvent>::Get();

        pEventBus->Subscribe(eInputEvents::KEYBOARD, InputTriggered);

        m_InitiateMouseCapture();
	}

	void PlayerControllerSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void PlayerControllerSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<KeyInputComponent> pKey = CoreLocator::GetKeyInput();
        for (Entity playerId : SceneView<PlayerComponent, TransformComponent, MovementComponent>(*pScene))
        {
            PlayerComponent& player = pScene->Get<PlayerComponent>(playerId);
            TransformComponent& transform = pScene->Get<TransformComponent>(playerId);
            MovementComponent& movement = pScene->Get<MovementComponent>(playerId);

            glm::vec3 playerRotation = TransformUtils::GetQuatAsDegrees(transform.orientation);

            glm::vec3 playerFront = glm::normalize(TransformUtils::GetForwardVector(transform.orientation));
            glm::vec3 playerSides = glm::normalize(glm::cross(playerFront, glm::vec3(UP_VECTOR)));

            glm::vec3 newVelocity = glm::vec3(0.0f);

            // Handle key presses for movement
            if (pKey->key[eKeyCodes::W])
            {
                newVelocity = player.speed * playerFront;
            }
            if (pKey->key[eKeyCodes::S])
            {
                newVelocity = -player.speed * playerFront;
            }
            if (pKey->key[eKeyCodes::A])
            {
                newVelocity = -player.speed * playerSides;
            }
            if (pKey->key[eKeyCodes::D])
            {
                newVelocity = player.speed * playerSides;
            }

            movement.velocity = newVelocity;

            // Mouse position for changing forward direction
            std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();

            float xoffset = (pMouse->posX - pMouse->lastPosX) * pMouse->sensitivity;

            // Create quaternions for yaw
            glm::quat yawQuat = glm::angleAxis(-xoffset, glm::vec3(0.0f, 1.0f, 0.0f));

            // Combine pitch and yaw quaternions
            glm::quat orientationChange = yawQuat;

            // Apply the combined quaternion to the player's orientation
            transform.orientation = glm::normalize(orientationChange * transform.orientation);

            pMouse->lastPosX = pMouse->posX;
            pMouse->lastPosY = pMouse->posY;
        }
	}

	void PlayerControllerSystem::Render(std::shared_ptr<Scene> pScene)
	{
	}

	void PlayerControllerSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void PlayerControllerSystem::Shutdown()
	{
        // Subscribe to keyboard event
        std::shared_ptr<iEventBus<eInputEvents, KeyboardEvent>> pEventBus = EventBusLocator<eInputEvents, KeyboardEvent>::Get();

        pEventBus->Unsubscribe(eInputEvents::KEYBOARD, InputTriggered);

        m_StopMouseCapture();
	}

    void PlayerControllerSystem::InputTriggered(const KeyboardEvent& event)
    {
        if (event.keyData.key != eKeyCodes::ESCAPE)
        {
            return;
        }

        // Trigger window close event
        std::shared_ptr<iEventBus<eWindowEvents, WindowCloseEvent>> pEventBus = EventBusLocator<eWindowEvents, WindowCloseEvent>::Get();

        WindowCloseEvent collEvent = WindowCloseEvent();
        pEventBus->Publish(collEvent);
    }

    void PlayerControllerSystem::m_InitiateMouseCapture()
    {
        // Set initial mouse position to consider mouse on center of window
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();
        if (pMouse->mouseCaptured)
        {
            return;
        }

        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        pMouse->lastPosX = pMouse->posX;
        pMouse->lastPosY = pMouse->posY;

        InputUtils::CaptureMouse(pWindow->pGLFWWindow);
        pMouse->mouseCaptured = true;
        pMouse->sensitivity = DEFAULT_MOUSE_SENSITIVITY;

        return;
    }

    void PlayerControllerSystem::m_StopMouseCapture()
    {
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();
        if (!pMouse->mouseCaptured)
        {
            return;
        }
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        InputUtils::ReleaseMouse(pWindow->pGLFWWindow);
        pMouse->mouseCaptured = false;

        return;
    }
}
