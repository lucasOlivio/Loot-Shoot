#include "pch.h"

#include "MaterialManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullMaterialManager> MaterialManagerLocator::m_nullService(new NullMaterialManager());
	std::shared_ptr<iMaterialManager> MaterialManagerLocator::m_pMaterialManager = m_nullService;

	std::shared_ptr<iMaterialManager> MaterialManagerLocator::Get()
	{
		return m_pMaterialManager;
	}

	void MaterialManagerLocator::Set(std::shared_ptr<iMaterialManager> pMaterialManager)
	{
		if (pMaterialManager == nullptr)
		{
			m_pMaterialManager = m_nullService;
		}
		else
		{
			m_pMaterialManager = pMaterialManager;
		}
	}
}
