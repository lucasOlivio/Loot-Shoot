#include "pch.h"

#include "ParticleManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullParticleManager> ParticleManagerLocator::m_nullService(new NullParticleManager());
	std::shared_ptr<iParticleManager> ParticleManagerLocator::m_pParticleManager = m_nullService;

	std::shared_ptr<iParticleManager> ParticleManagerLocator::Get()
	{
		return m_pParticleManager;
	}

	void ParticleManagerLocator::Set(std::shared_ptr<iParticleManager> pParticleManager)
	{
		if (pParticleManager == nullptr)
		{
			m_pParticleManager = m_nullService;
		}
		else
		{
			m_pParticleManager = pParticleManager;
		}
	}
}
