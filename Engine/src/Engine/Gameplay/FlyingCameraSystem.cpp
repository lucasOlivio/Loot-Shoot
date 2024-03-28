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

        pMouse->LockWrite();
        pMouse->sensitivity = DEFAULT_MOUSE_SENSITIVITY;
        pMouse->UnlockWrite();
    }

    void FlyingCameraSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();

        pMouse->LockRead();
        bool isMouseBtn1 = pMouse->button[eMouseCodes::MOUSE_BUTTON_1];
        pMouse->UnlockRead();
        if (isMouseBtn1)
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

    void FlyingCameraSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
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
        
        pMouse->LockRead();
        bool isMouseCaptured = pMouse->mouseCaptured;
        pMouse->UnlockRead();
        if (isMouseCaptured)
        {
            pMouse->LockWrite();
            return;
        }

        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        pWindow->LockRead();
        InputUtils::CaptureMouse(pWindow->pGLFWWindow);
        pWindow->UnlockRead();

        pMouse->LockWrite();
        pMouse->lastPosX = pMouse->posX;
        pMouse->lastPosY = pMouse->posY;
        pMouse->mouseCaptured = true;
        pMouse->UnlockWrite();

        return;
    }

    void FlyingCameraSystem::m_StopMouseCapture()
    {
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();

        pMouse->LockRead();
        bool isMouseCaptured = pMouse->mouseCaptured;
        pMouse->UnlockRead();
        if (!isMouseCaptured)
        {
            return;
        }
        std::shared_ptr<WindowComponent> pWindow = GraphicsLocator::GetWindow();

        pWindow->LockRead();
        InputUtils::ReleaseMouse(pWindow->pGLFWWindow);
        pWindow->UnlockRead();

        pMouse->LockWrite();
        pMouse->mouseCaptured = false;
        pMouse->UnlockWrite();

        return;
    }

    void FlyingCameraSystem::m_UpdateCameraRotation(std::shared_ptr<Scene> pScene)
    {
        Entity cameraId = CameraUtils::GetMainCamera(pScene);

        TransformComponent& cameraTransform = pScene->Get<TransformComponent>(cameraId);
        std::shared_ptr<MouseInputComponent> pMouse = CoreLocator::GetMouseInput();

        pMouse->LockRead();
        float xoffset = (pMouse->posX - pMouse->lastPosX) * pMouse->sensitivity;
        float yoffset = (pMouse->posY - pMouse->lastPosY) * pMouse->sensitivity;
        pMouse->UnlockRead();

        cameraTransform.LockRead();
        glm::vec3 cameraRight = glm::normalize(TransformUtils::GetRightVector(cameraTransform.orientation));
        cameraTransform.UnlockRead();

        // Create quaternions for pitch and yaw
        glm::quat pitchQuat = glm::angleAxis(-yoffset, cameraRight);
        glm::quat yawQuat = glm::angleAxis(-xoffset, glm::vec3(0.0f, 1.0f, 0.0f));

        // Combine pitch and yaw quaternions
        glm::quat orientationChange = yawQuat * pitchQuat;

        // Apply the combined quaternion to the camera's orientation
        cameraTransform.LockWrite();
        cameraTransform.orientation = glm::normalize(orientationChange * cameraTransform.orientation);
        cameraTransform.UnlockWrite();

        pMouse->LockWrite();
        pMouse->lastPosX = pMouse->posX;
        pMouse->lastPosY = pMouse->posY;
        pMouse->UnlockWrite();
    }

    void FlyingCameraSystem::m_UpdateCameraPosition(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        Entity cameraId = CameraUtils::GetMainCamera(pScene);

        TransformComponent& cameraTransform = pScene->Get<TransformComponent>(cameraId);
        std::shared_ptr<KeyInputComponent> pKey = CoreLocator::GetKeyInput();

        cameraTransform.LockRead();
        glm::vec3 cameraRotation = TransformUtils::GetQuatAsDegrees(cameraTransform.orientation);
        glm::vec3 cameraFront = glm::normalize(TransformUtils::GetForwardVector(cameraTransform.orientation));
        glm::vec3 cameraSides = glm::normalize(glm::cross(cameraFront, glm::vec3(UP_VECTOR)));
        glm::vec3 cameraUp = glm::normalize(TransformUtils::GetUpVector(cameraTransform.orientation));
        cameraTransform.UnlockRead();

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

        cameraTransform.LockWrite();
        cameraTransform.position += moveOffset;
        cameraTransform.UnlockWrite();
    }
}
