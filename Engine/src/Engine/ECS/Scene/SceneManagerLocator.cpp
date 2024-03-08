#include "pch.h"

#include "SceneManagerLocator.h"

namespace MyEngine
{
	std::shared_ptr<NullSceneManager> SceneManagerLocator::m_nullService(new NullSceneManager());
	std::shared_ptr<iSceneManager> SceneManagerLocator::m_pSceneManager = m_nullService;

	std::shared_ptr<iSceneManager> SceneManagerLocator::Get()
	{
		return m_pSceneManager;
	}

	void SceneManagerLocator::Set(std::shared_ptr<iSceneManager> pSceneManager)
	{
		if (pSceneManager == nullptr)
		{
			m_pSceneManager = m_nullService;
		}
		else
		{
			m_pSceneManager = pSceneManager;
		}
	}
}
