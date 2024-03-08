#include "pch.h"

#include "ShaderManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullShaderManager> ShaderManagerLocator::m_nullService(new NullShaderManager());
	std::shared_ptr<iShaderManager> ShaderManagerLocator::m_pShaderManager = m_nullService;

	std::shared_ptr<iShaderManager> ShaderManagerLocator::Get()
	{
		return m_pShaderManager;
	}

	void ShaderManagerLocator::Set(std::shared_ptr<iShaderManager> pShaderManager)
	{
		if (pShaderManager == nullptr)
		{
			m_pShaderManager = m_nullService;
		}
		else
		{
			m_pShaderManager = pShaderManager;
		}
	}
}
