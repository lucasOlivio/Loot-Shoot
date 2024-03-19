#include "pch.h"

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

	void ParticleManager::ResetParticles()
	{
		for (ParticleProps& particle : m_vecParticles)
		{
			particle.lifetime = 0.0f;
		}

		m_nextParticle = 0;
	}
}