#include "pch.h"

#include "TextureManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullTextureManager> TextureManagerLocator::m_nullService(new NullTextureManager());
	std::shared_ptr<iTextureManager> TextureManagerLocator::m_pTextureManager = m_nullService;

	std::shared_ptr<iTextureManager> TextureManagerLocator::Get()
	{
		return m_pTextureManager;
	}

	void TextureManagerLocator::Set(std::shared_ptr<iTextureManager> pTextureManager)
	{
		if (pTextureManager == nullptr)
		{
			m_pTextureManager = m_nullService;
		}
		else
		{
			m_pTextureManager = pTextureManager;
		}
	}
}
