#include "pch.h"

#include "ThreadPoolLocator.h"

#include "ThreadPool.h"

namespace MyEngine
{
	using itPools = std::map<std::string, std::shared_ptr<iThreadPool>>::iterator;
	using pairPools = std::pair<std::string, std::shared_ptr<iThreadPool>>;

	std::map<std::string, std::shared_ptr<iThreadPool>> ThreadPoolLocator::m_mapThreadPool = {};

	std::shared_ptr<iThreadPool> ThreadPoolLocator::GetOrCreate(const std::string& poolName)
	{
		itPools it = m_mapThreadPool.find(poolName);
		if (it == m_mapThreadPool.end())
		{
			std::shared_ptr<iThreadPool> pThreadPool = std::shared_ptr<iThreadPool>(new ThreadPool());
			pThreadPool->CreateWorkers();

			m_mapThreadPool.emplace(poolName, pThreadPool);

			it = m_mapThreadPool.find(poolName);
		}

		std::shared_ptr<iThreadPool> pThreadPool = it->second;

		return pThreadPool;
	}

	void ThreadPoolLocator::CloseAllPools()
	{
		for (const pairPools& pool : m_mapThreadPool)
		{
			pool.second->CloseThreads();
		}
	}
}
