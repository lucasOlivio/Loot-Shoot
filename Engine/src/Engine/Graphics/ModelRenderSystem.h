#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	// Sends all needed models to the rendering pipeline
	class ModelRenderSystem : public EntitySystem
	{
	public:
		ModelRenderSystem() = default;
		virtual ~ModelRenderSystem() { };

		virtual std::string SystemName() { return "ModelRenderSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);
	};
}
