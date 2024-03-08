#pragma once

#include "NullShaderManager.hpp"
#include "iShaderManager.h"

namespace MyEngine
{
	class ShaderManagerLocator
	{
	public:
		static std::shared_ptr<iShaderManager> Get();

		static void Set(std::shared_ptr<iShaderManager> pShaderManager);

	private:
		static std::shared_ptr<iShaderManager> m_pShaderManager;
		static std::shared_ptr<NullShaderManager> m_nullService;
	};
}
