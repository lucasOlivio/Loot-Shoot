#pragma once

#include "Engine/Graphics/opengl.h"
#include "Engine/Graphics/Particles/iParticleManager.h"

namespace MyEngine
{
	class ParticleManager : public iParticleManager
	{
	public:
		ParticleManager();
		virtual ~ParticleManager();

		// Setup the buffers and attributes for the shader
		virtual void Initialize();

		// Returns all the particles alive or not
		virtual std::vector<ParticleProps>& GetParticles();

		// Get the next available particle
		virtual ParticleProps& EmitParticle();

		// Update the values for the corresponding particle
		virtual void UpdateParticle(uint index, const ParticleProps& props);

		// Send the particle to the rendering pipeline
		virtual void SendToDraw(GLuint numTexture, const ParticleProps& props);

		// Check if the particles were already drawed and cleared
		virtual bool ReadyToDraw();

		// Update the buffer data and draw particles instanced
		virtual void DrawParticles();
		virtual void ClearDraw();
		virtual void SetReadyToDraw(bool setReady);

		// Reset all particles life to 0
		virtual void ResetParticles();

	private:
		std::vector<ParticleProps> m_vecParticles;
		std::map<GLuint, std::vector<ParticleProps>> m_mapParticlesToDraw;

		size_t m_nextParticle = 0;
		bool m_isReadyToDraw;

		const std::string PARTICLE_MESH = "flat.ply";

		std::shared_ptr<sMeshInfo> m_pParticleMesh;
		uint m_instanceVBO;
	};
}
