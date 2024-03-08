#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	// Draw the whole AABB grid from broadphase
	class DrawGridSystem : public iSystem
	{
	public:
		DrawGridSystem() = default;
		virtual ~DrawGridSystem() { };

		virtual std::string SystemName() { return "DrawGridSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
