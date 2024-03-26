#include "pch.h"

#include "Engine/Core/Resources/Meshes/MeshManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"
#include "Engine/Core/Resources/Textures/TextureManager.h"

#include "Engine/Graphics/Particles/ParticleManager.h"

namespace MyEngine
{
	using itParticlesDraw = std::map<GLuint, std::vector<ParticleProps>>::iterator;
	using pairParticlesDraw = std::pair<GLuint, std::vector<ParticleProps>>;

	ParticleManager::ParticleManager() : m_isReadyToDraw(false)
	{
	}

	ParticleManager::~ParticleManager()
	{
	}

	void ParticleManager::Initialize()
	{
		// Load particle mesh and buffer arrays
		std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		pShaderManager->ActivateResource(INSTANCING_SHADER);

		std::shared_ptr<MeshManager> pMeshManager = ResourceManagerFactory::GetOrCreate<MeshManager>(eResourceTypes::MESH);
		m_pParticleMesh = pMeshManager->LoadParticles(PARTICLE_MESH, m_vecParticles, m_instanceVBO);
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

	void ParticleManager::SendToDraw(GLuint numTexture, const ParticleProps& props)
	{
		LockWrite();
		itParticlesDraw it = m_mapParticlesToDraw.find(numTexture);
		if (it == m_mapParticlesToDraw.end())
		{
			m_mapParticlesToDraw[numTexture] = {};
			it = m_mapParticlesToDraw.find(numTexture);
		}
		it->second.push_back(props);
		UnlockWrite();
	}

	bool ParticleManager::ReadyToDraw()
	{
		return m_isReadyToDraw;
	}

	void ParticleManager::DrawParticles()
	{
		while (!ReadyToDraw())
		{
			Sleep(0);
		}

		std::shared_ptr<ShaderManager> pShaderManager = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);
		
		glDisable(GL_CULL_FACE);
		glBindVertexArray(m_pParticleMesh->VAO_ID);
		glActiveTexture(GL_TEXTURE0);
		pShaderManager->SetUniformInt("textureColor", GL_TEXTURE0);

		// Bind the VBO to update its data
		glBindBuffer(GL_ARRAY_BUFFER, m_instanceVBO);

		LockRead();
		for (itParticlesDraw it = m_mapParticlesToDraw.begin(); it != m_mapParticlesToDraw.end(); it++)
		{
			size_t numParticles = it->second.size();

			glBindTexture(GL_TEXTURE_2D, it->first);
			
			// Update the VBO with the new data
			glBufferSubData(GL_ARRAY_BUFFER, 0,
							(sizeof(ParticleProps) * numParticles),
							&(it->second[0]));

			glDrawElementsInstanced(
				GL_TRIANGLES, m_pParticleMesh->numberOfIndices, GL_UNSIGNED_INT, 0, numParticles
			);
		}
		UnlockRead();

		// Unbind the VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		glEnable(GL_CULL_FACE);

		ClearDraw();
	}

	void ParticleManager::ClearDraw()
	{
		LockWrite();
		for (itParticlesDraw it = m_mapParticlesToDraw.begin(); it != m_mapParticlesToDraw.end(); it++)
		{
			it->second.clear();
		}
		m_mapParticlesToDraw.clear();
		UnlockWrite();

		SetReadyToDraw(false);
	}

	void ParticleManager::SetReadyToDraw(bool setReady)
	{
		m_isReadyToDraw = setReady;
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