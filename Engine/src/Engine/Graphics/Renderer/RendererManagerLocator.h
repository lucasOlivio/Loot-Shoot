#pragma once

#include "NullRendererManager.hpp"
#include "iRendererManager.h"

namespace MyEngine
{
	class RendererManagerLocator
	{
	public:
		static std::shared_ptr<iRendererManager> Get();

		static void Set(std::shared_ptr<iRendererManager> pRendererManager);

	private:
		static std::shared_ptr<iRendererManager> m_pRendererManager;
		static std::shared_ptr<NullRendererManager> m_nullService;
	};
}
