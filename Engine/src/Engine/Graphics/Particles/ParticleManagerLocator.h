#pragma once

#include "NullParticleManager.hpp"
#include "iParticleManager.h"

namespace MyEngine
{
	class ParticleManagerLocator
	{
	public:
		static std::shared_ptr<iParticleManager> Get();

		static void Set(std::shared_ptr<iParticleManager> pParticleManager);

	private:
		static std::shared_ptr<iParticleManager> m_pParticleManager;
		static std::shared_ptr<NullParticleManager> m_nullService;
	};
}
