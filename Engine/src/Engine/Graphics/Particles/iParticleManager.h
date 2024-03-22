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

		// Setup the buffers and attributes for the shader
		virtual void Initialize() = 0;

		// Returns all the particles alive or not
		virtual std::vector<ParticleProps>& GetParticles() = 0;

		// Get the next available particle
		virtual ParticleProps& EmitParticle() = 0;

		// Update the values for the corresponding particle
		virtual void UpdateParticle(uint index, const ParticleProps& props) = 0;

		// Send the particle to the rendering pipeline
		virtual void SendToDraw(GLuint numTexture, const ParticleProps& props) = 0;
		
		// Check if the particles were already drawed and cleared
		virtual bool ReadyToDraw() = 0;
		virtual void ClearDraw() = 0;
		virtual void SetReadyToDraw(bool setReady) = 0;

		// Update the buffer data and draw particles instanced
		virtual void DrawParticles() = 0;

		// Reset all particles life to 0
		virtual void ResetParticles() = 0;
	};
}
