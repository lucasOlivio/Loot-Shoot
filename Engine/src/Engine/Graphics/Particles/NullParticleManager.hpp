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

		// Send the particle to the rendering pipeline
		virtual void SendToDraw(GLuint numTexture, const ParticleProps& props)
		{
			LOG_ERROR("Particle manager not initialized!");
		}

		// Check if the particles were already drawed and cleared
		virtual bool ReadyToDraw()
		{
			LOG_ERROR("Particle manager not initialized!");

			return false;
		}

		virtual void ClearDraw()
		{
			LOG_ERROR("Particle manager not initialized!");
		}

		virtual void SetReadyToDraw(bool setReady)
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
