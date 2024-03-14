#include "pch.h"

#include "ThreadPoolLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullThreadPool> ThreadPoolLocator::m_nullService(new NullThreadPool());
	std::shared_ptr<iThreadPool> ThreadPoolLocator::m_pThreadPool = m_nullService;

	std::shared_ptr<iThreadPool> ThreadPoolLocator::Get()
	{
		return m_pThreadPool;
	}

	void ThreadPoolLocator::Set(std::shared_ptr<iThreadPool> pThreadPool)
	{
		if (pThreadPool == nullptr)
		{
			m_pThreadPool = m_nullService;
		}
		else
		{
			m_pThreadPool = pThreadPool;
		}
	}
}
