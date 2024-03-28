#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	class MaterialOffsetSystem : public iSystem
	{
	public:
		MaterialOffsetSystem() = default;
		virtual ~MaterialOffsetSystem() { };

		virtual std::string SystemName() { return "MaterialOffsetSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
