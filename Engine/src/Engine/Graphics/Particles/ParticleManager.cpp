#include "pch.h"

#include "Engine/Core/Resources/Meshes/MeshManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include "Engine/Graphics/Particles/ParticleManager.h"

namespace MyEngine
{
	ParticleManager::ParticleManager()
	{
		m_vecParticles.resize(MAX_PARTICLES);
	}

	ParticleManager::~ParticleManager()
	{
	}

	void ParticleManager::Initialize()
	{
		std::shared_ptr<iResourceManager> pShaderManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::SHADER);
		pShaderManager->ActivateResource(INSTANCING_SHADER);

		std::shared_ptr<MeshManager> pMeshManager = ResourceManagerFactory::GetOrCreate<MeshManager>(eResourceTypes::MESH);
		m_pParticleMesh = pMeshManager->LoadParticles(PARTICLE_MESH, m_vecParticles, m_instanceVBO);

		pShaderManager->ActivateResource(DEFAULT_SHADER);
	}

	std::vector<ParticleProps>& ParticleManager::GetParticles()
	{
		return m_vecParticles;
	}

	ParticleProps& ParticleManager::EmitParticle()
	{
		ParticleProps& particle = m_vecParticles[m_nextParticle];
		m_nextParticle = ++m_nextParticle % m_vecParticles.size();

		return particle;
	}

	void ParticleManager::UpdateParticle(uint index, const ParticleProps& props)
	{
		ParticleProps& particle = m_vecParticles[index];
		particle.LockWrite();
		particle = props;
		particle.UnlockWrite();
	}

	void ParticleManager::DrawParticles()
	{
		glDisable(GL_CULL_FACE);

		glBindVertexArray(m_pParticleMesh->VAO_ID);

		// Bind the VBO to update its data
		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

		LockRead();
		// Update the VBO with the new data
		glBufferSubData(GL_ARRAY_BUFFER, 0,
			sizeof(ParticleProps) * MAX_PARTICLES,
			&(m_vecParticles[0]));
		UnlockRead();

		glDrawElementsInstanced(
			GL_TRIANGLES, m_pParticleMesh->numberOfIndices, GL_UNSIGNED_INT, 0, MAX_PARTICLES
		);

		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_CULL_FACE);
	}

	void ParticleManager::ResetParticles()
	{
		for (ParticleProps& particle : m_vecParticles)
		{
			particle.lifetime = 0.0f;
		}

		m_nextParticle = 0;
	}
}