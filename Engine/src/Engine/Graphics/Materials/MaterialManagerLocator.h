#pragma once

#include "NullMaterialManager.hpp"
#include "iMaterialManager.h"

namespace MyEngine
{
	class MaterialManagerLocator
	{
	public:
		static std::shared_ptr<iMaterialManager> Get();

		static void Set(std::shared_ptr<iMaterialManager> pMaterialManager);

	private:
		static std::shared_ptr<iMaterialManager> m_pMaterialManager;
		static std::shared_ptr<NullMaterialManager> m_nullService;
	};
}
