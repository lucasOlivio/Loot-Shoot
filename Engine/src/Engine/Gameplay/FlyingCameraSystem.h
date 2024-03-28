#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Captures mouse and moves the camera around accordingly to keyboard inputs and mouse position
	// Starts moving when mouse button 1 is pressed
	class FlyingCameraSystem : public iSystem
	{
	public:
		FlyingCameraSystem() = default;
		virtual ~FlyingCameraSystem() { };

		virtual std::string SystemName() { return "FlyingCameraSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	private:
		void m_InitiateMouseCapture();

		void m_StopMouseCapture();

		void m_UpdateCameraRotation(std::shared_ptr<Scene> pScene);

		void m_UpdateCameraPosition(std::shared_ptr<Scene> pScene, float deltaTime);
	};
}
