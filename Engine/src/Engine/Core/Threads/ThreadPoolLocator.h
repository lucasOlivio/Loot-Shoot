#pragma once

#include "NullThreadPool.hpp"
#include "iThreadPool.h"

namespace MyEngine
{
	class ThreadPoolLocator
	{
	public:
		static std::shared_ptr<iThreadPool> Get();

		static void Set(std::shared_ptr<iThreadPool> pThreadPool);

	private:
		static std::shared_ptr<iThreadPool> m_pThreadPool;
		static std::shared_ptr<NullThreadPool> m_nullService;
	};
}
