#pragma once

#include "NullTextureManager.hpp"
#include "iTextureManager.h"

namespace MyEngine
{
	class TextureManagerLocator
	{
	public:
		static std::shared_ptr<iTextureManager> Get();

		static void Set(std::shared_ptr<iTextureManager> pTextureManager);

	private:
		static std::shared_ptr<iTextureManager> m_pTextureManager;
		static std::shared_ptr<NullTextureManager> m_nullService;
	};
}
