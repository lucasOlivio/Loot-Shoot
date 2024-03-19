#pragma once

#include "NullThreadPool.hpp"
#include "iThreadPool.h"

#include <map>

namespace MyEngine
{
	class ThreadPoolLocator
	{
	public:
		static std::shared_ptr<iThreadPool> GetOrCreate(const std::string& poolName);

		static void CloseAllPools();

	private:
		static std::map<std::string, std::shared_ptr<iThreadPool>> m_mapThreadPool;
	};
}
