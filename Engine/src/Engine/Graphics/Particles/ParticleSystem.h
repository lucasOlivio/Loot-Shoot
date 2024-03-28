#pragma once

#include "Engine/Core/Components/Components.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include "Engine/ECS/System/EntitySystem.h"

#include "Engine/Graphics/Components/Components.h"

namespace MyEngine
{
	// Deals with the particles managements through the emitters and the shaders
	class ParticleSystem : public EntitySystem
	{
	public:
		ParticleSystem() = default;
		virtual ~ParticleSystem() { };

		virtual std::string SystemName() { return "ParticleSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

		virtual void SetSystemMask(std::shared_ptr<Scene> pScene);

	private:
		void m_SetEmitterUniforms(TransformComponent& transform, 
								  EmitterComponent& emitter, 
								  std::shared_ptr<ShaderManager> pShaderManager,
								  uint particlesToSpawn);

		void m_ParticlesUpdate(std::shared_ptr<Scene> pScene, float deltaTime);

		void m_ParticlesRender(std::shared_ptr<Scene> pScene);
	};
}
