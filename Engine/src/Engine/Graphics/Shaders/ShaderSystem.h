#pragma once

#include "Engine/ECS/System/iSystem.h"

namespace MyEngine
{
	class ShaderSystem : public iSystem
	{
	public:
		ShaderSystem() = default;
		virtual ~ShaderSystem() { };

		virtual std::string SystemName() { return "ShaderSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();
	};
}
