#pragma once

#include "Engine/Utils/Log.h"
#include "Engine/Graphics/Particles/iParticleManager.h"

namespace MyEngine
{
	class NullParticleManager : public iParticleManager
	{
	public:
		NullParticleManager() {};
		virtual ~NullParticleManager() {};

		// Setup the buffers and attributes for the shader
		virtual void Initialize()
		{
			LOG_ERROR("Particle manager not initialized!");
		}

		// Returns all the particles alive or not
		virtual std::vector<ParticleProps>& GetParticles()
		{
			LOG_ERROR("Particle manager not initialized!");
			return emptyVec;
		}

		// Sets the necessary parameters for the next available particle
		virtual ParticleProps& EmitParticle()
		{
			LOG_ERROR("Particle manager not initialized!");
			return emptyParticle;
		}

		// Update the values for the corresponding particle
		virtual void UpdateParticle(uint index, const ParticleProps& props)
		{
			LOG_ERROR("Particle manager not initialized!");
		}

		// Reset all particles life to 0
		virtual void ResetParticles()
		{
			LOG_ERROR("Particle manager not initialized!");
		}

		virtual void DrawParticles()
		{
			LOG_ERROR("Particle manager not initialized!");
		}

		std::vector<ParticleProps> emptyVec = {};
		ParticleProps emptyParticle = ParticleProps();
	};
}
