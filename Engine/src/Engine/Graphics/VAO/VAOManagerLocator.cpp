#include "pch.h"

#include "VAOManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullVAOManager> VAOManagerLocator::m_nullService(new NullVAOManager());
	std::shared_ptr<iVAOManager> VAOManagerLocator::m_pVAOManager = m_nullService;

	std::shared_ptr<iVAOManager> VAOManagerLocator::Get()
	{
		return m_pVAOManager;
	}

	void VAOManagerLocator::Set(std::shared_ptr<iVAOManager> pVAOManager)
	{
		if (pVAOManager == nullptr)
		{
			m_pVAOManager = m_nullService;
		}
		else
		{
			m_pVAOManager = pVAOManager;
		}
	}
}
