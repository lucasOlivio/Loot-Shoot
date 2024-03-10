#pragma once

#include "iResourceManager.h"

namespace MyEngine
{
	class ResourceManagerFactory
	{
	public:
		ResourceManagerFactory() {};
		~ResourceManagerFactory() {};

		static std::shared_ptr<iResourceManager> GetOrCreate(const eResourceTypes& resType);

		template<typename T>
		static std::shared_ptr<T> GetOrCreate(const eResourceTypes& resType)
		{
			return std::static_pointer_cast<T>(GetOrCreate(resType));
		}
	};
}
