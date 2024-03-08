#pragma once

#include "NullVAOManager.hpp"
#include "iVAOManager.h"

namespace MyEngine
{
	class VAOManagerLocator
	{
	public:
		static std::shared_ptr<iVAOManager> Get();

		static void Set(std::shared_ptr<iVAOManager> pVAOManager);

	private:
		static std::shared_ptr<iVAOManager> m_pVAOManager;
		static std::shared_ptr<NullVAOManager> m_nullService;
	};
}
