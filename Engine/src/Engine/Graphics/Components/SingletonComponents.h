#pragma once

#include "Engine/ECS/BaseComponent.h"

#include "Engine/Graphics/GraphicsProperties.h"

#include <glm/glm.hpp>
#include <string>

namespace MyEngine
{
	struct WindowComponent : public BaseComponent
	{
		std::string prevName = ""; // Only for window system to change
		std::string name;
		int width = 640;
		int height = 480;

		GLFWwindow* pGLFWWindow;
	};

	struct CameraComponent : public BaseComponent
	{
		glm::vec3 upVector;

		float distance;
		float height;
		float offsetTarget;

		float fovy;
		float zNear;
		float zFar;
	};

	struct AnimationControllerComponent : public BaseComponent
	{
		float timeLastKeyFrame = 0.0f;
		float timeFirstKeyFrame = FLT_MAX;

		float speed = 1.0f;
		bool reset = false;
	};
}
