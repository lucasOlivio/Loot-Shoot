#include "pch.h"

#include "RendererManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullRendererManager> RendererManagerLocator::m_nullService(new NullRendererManager());
	std::shared_ptr<iRendererManager> RendererManagerLocator::m_pRendererManager = m_nullService;

	std::shared_ptr<iRendererManager> RendererManagerLocator::Get()
	{
		return m_pRendererManager;
	}

	void RendererManagerLocator::Set(std::shared_ptr<iRendererManager> pRendererManager)
	{
		if (pRendererManager == nullptr)
		{
			m_pRendererManager = m_nullService;
		}
		else
		{
			m_pRendererManager = pRendererManager;
		}
	}
}
