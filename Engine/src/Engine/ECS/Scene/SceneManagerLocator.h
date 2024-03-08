#pragma once

#include "NullSceneManager.hpp"
#include "iSceneManager.h"

namespace MyEngine
{
	class SceneManagerLocator
	{
	public:
		static std::shared_ptr<iSceneManager> Get();

		static void Set(std::shared_ptr<iSceneManager> pSceneManager);

	private:
		static std::shared_ptr<iSceneManager> m_pSceneManager;
		static std::shared_ptr<NullSceneManager> m_nullService;
	};
}
