#pragma once

#include "Engine/ECS/System/EntitySystem.h"

namespace MyEngine
{
	// Deals with the particles setup and creation
	class ParticleEmissionSystem : public EntitySystem
	{
	public:
		ParticleEmissionSystem() = default;
		virtual ~ParticleEmissionSystem() { };

		virtual std::string SystemName() { return "ParticleEmissionSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);
	};
}
