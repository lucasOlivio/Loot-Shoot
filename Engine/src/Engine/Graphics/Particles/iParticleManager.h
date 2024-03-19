#pragma once

#include "Engine/Graphics/Particles/ParticlesProperties.h"

#include "Engine/ThreadSafe.h"

namespace MyEngine
{
	class iParticleManager : public ThreadSafe
	{
	public:
		iParticleManager() : ThreadSafe() {};
		virtual ~iParticleManager() {};

		// Returns all the particles alive or not
		virtual std::vector<ParticleProps>& GetParticles() = 0;

		// Get the next available particle
		virtual ParticleProps& EmitParticle() = 0;

		// Update the values for the corresponding particle
		virtual void UpdateParticle(uint index, const ParticleProps& props) = 0;

		// Reset all particles life to 0
		virtual void ResetParticles() = 0;
	};
}
