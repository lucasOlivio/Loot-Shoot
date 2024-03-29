#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Main top bar menu UI
	class MenuSystem : public iSystem
	{
	public:
		MenuSystem() = default;
		virtual ~MenuSystem() { };

		virtual std::string SystemName() { return "MenuSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	private:
		void m_LoadScene(bool openLoadScene);

		void m_NewScene(bool openNewScene);

		void m_SaveScene();

		void m_PlayScene();

		void m_StopScene();
	};
}
