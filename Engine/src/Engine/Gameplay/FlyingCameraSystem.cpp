#include "pch.h"

#include "FlyingCameraSystem.h"

#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Components/Components.h"

#include "Engine/Graphics/Components/GraphicsLocator.h"

#include "Engine/Core/InputProperties.h"

#include "Engine/Utils/CameraUtils.h"
#include "Engine/Utils/InputUtils.h"
#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/Math.h"

#define DEFAULT_MOUSE_SENSITIVITY 0.001f
#define DEFAULT_CAMERA_SPEED 1000.0f

namespace MyEngine
{
    void FlyingCameraSystem::Init()
    {
    }

    void FlyingCameraSystem::Start(std::shared_ptr<Scene> pScene)
    {
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();
        pMouse->sensitivity = DEFAULT_MOUSE_SENSITIVITY;
    }

    void FlyingCameraSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();

        if (pMouse->button[eMouseCodes::MOUSE_BUTTON_1])
        {
            m_InitiateMouseCapture();

            // Update camera transform
            m_UpdateCameraRotation(pScene);
            m_UpdateCameraPosition(pScene, deltaTime);
        }
        else
        {
            m_StopMouseCapture();
        }
    }

    void FlyingCameraSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void FlyingCameraSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void FlyingCameraSystem::Shutdown()
    {
    }

    void FlyingCameraSystem::m_InitiateMouseCapture()
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

        return;
    }

    void FlyingCameraSystem::m_StopMouseCapture()
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

    void FlyingCameraSystem::m_UpdateCameraRotation(std::shared_ptr<Scene> pScene)
    {
        Entity cameraId = CameraUtils::GetMainCamera(pScene);

        CameraComponent& camera = pScene->Get<CameraComponent>(cameraId);
        TransformComponent& cameraTransform = pScene->Get<TransformComponent>(cameraId);
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();

        float xoffset = (pMouse->posX - pMouse->lastPosX) * pMouse->sensitivity;
        float yoffset = (pMouse->posY - pMouse->lastPosY) * pMouse->sensitivity;

        glm::vec3 cameraRight = glm::normalize(TransformUtils::GetRightVector(cameraTransform.orientation));

        // Create quaternions for pitch and yaw
        glm::quat pitchQuat = glm::angleAxis(-yoffset, cameraRight);
        glm::quat yawQuat = glm::angleAxis(-xoffset, glm::vec3(0.0f, 1.0f, 0.0f));

        // Combine pitch and yaw quaternions
        glm::quat orientationChange = yawQuat * pitchQuat;

        // Apply the combined quaternion to the camera's orientation
        cameraTransform.orientation = glm::normalize(orientationChange * cameraTransform.orientation);

        pMouse->lastPosX = pMouse->posX;
        pMouse->lastPosY = pMouse->posY;
    }

    void FlyingCameraSystem::m_UpdateCameraPosition(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        Entity cameraId = CameraUtils::GetMainCamera(pScene);

        CameraComponent& camera = pScene->Get<CameraComponent>(cameraId);
        TransformComponent& cameraTransform = pScene->Get<TransformComponent>(cameraId);
        std::shared_ptr<KeyInputComponent> pKey = CoreLocator::GetKeyInput();

        glm::vec3 cameraRotation = TransformUtils::GetQuatAsDegrees(cameraTransform.orientation);

        glm::vec3 cameraFront = glm::normalize(TransformUtils::GetForwardVector(cameraTransform.orientation));
        glm::vec3 cameraSides = glm::normalize(glm::cross(cameraFront, glm::vec3(UP_VECTOR)));
        glm::vec3 cameraUp = glm::normalize(TransformUtils::GetUpVector(cameraTransform.orientation));

        glm::vec3 moveOffset = glm::vec3(0.0f);

        // Handle key presses for movement
        if (pKey->key[eKeyCodes::W])
        {
            moveOffset = DEFAULT_CAMERA_SPEED * cameraFront * deltaTime;
        }
        if (pKey->key[eKeyCodes::S])
        {
            moveOffset = -(DEFAULT_CAMERA_SPEED * cameraFront * deltaTime);
        }
        if (pKey->key[eKeyCodes::A])
        {
            moveOffset = -(DEFAULT_CAMERA_SPEED * cameraSides * deltaTime);
        }
        if (pKey->key[eKeyCodes::D])
        {
            moveOffset = DEFAULT_CAMERA_SPEED * cameraSides * deltaTime;
        }
        if (pKey->key[eKeyCodes::Q])
        {
            moveOffset = -(DEFAULT_CAMERA_SPEED * cameraUp * deltaTime);
        }
        if (pKey->key[eKeyCodes::E])
        {
            moveOffset = DEFAULT_CAMERA_SPEED * cameraUp * deltaTime;
        }

        cameraTransform.position += moveOffset;
    }
}
