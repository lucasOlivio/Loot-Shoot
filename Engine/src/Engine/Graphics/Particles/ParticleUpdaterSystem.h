#pragma once

#include "Engine/ECS/System/iSystem.h"

#include "Engine/Graphics/Particles/ParticlesProperties.h"

namespace MyEngine
{
	// Processes the particles attributes every frame
	class ParticleUpdaterSystem : public iSystem
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

		static void UpdateParticles(ParticleProps* vecParticles,
								 size_t startIndex, size_t endIndex,
								 std::shared_ptr<Scene> pScene, float deltaTime);
	};
}
