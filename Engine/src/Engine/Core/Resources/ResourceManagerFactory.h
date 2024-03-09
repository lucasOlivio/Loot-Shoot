#pragma once

#include "iResourceManager.h"

namespace MyEngine
{
	class ResourceManagerFactory
	{
	public:
		ResourceManagerFactory() {};
		~ResourceManagerFactory() {};

		static std::shared_ptr<iResourceManager> CreateResManager(const eResourceTypes& resType);
	};
}
