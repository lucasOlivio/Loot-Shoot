#pragma once

#include "Engine/ECS/System/EntitySystem.h"

#include "Engine/Graphics/Particles/iParticleManager.h"
#include "Engine/Graphics/Particles/ParticlesProperties.h"
#include "Engine/Graphics/Components/Components.h"

namespace MyEngine
{
	// Processes the particles attributes every frame
	class ParticleUpdaterSystem : public EntitySystem
	{
	public:
		ParticleUpdaterSystem() = default;
		virtual ~ParticleUpdaterSystem() { };

		virtual std::string SystemName() { return "ParticleUpdaterSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);
	};
}
