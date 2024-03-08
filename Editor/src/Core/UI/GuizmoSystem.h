#pragma once

#include "Engine/ECS/System/iSystem.h"

#include "Engine/Events/SceneEvent.h"

namespace MyEngine
{
	// Deals with guizmo drawing and modes
	class GuizmoSystem : public iSystem
	{
	public:
		GuizmoSystem() = default;
		virtual ~GuizmoSystem() { };

		virtual std::string SystemName() { return "GuizmoSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
