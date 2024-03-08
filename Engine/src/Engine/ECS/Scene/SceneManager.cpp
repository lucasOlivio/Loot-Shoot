#include "pch.h"

#include "SceneManager.h"

#include "Engine/ECS/Scene/SceneSerializerFactory.h"
#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Events/SceneEvent.h"
#include "Engine/Events/EventBusLocator.hpp"

namespace MyEngine
{
	using itScenes = std::map<std::string /*Scene name*/, std::shared_ptr<Scene>>::iterator;
	using pairScenes = std::pair<std::string /*Scene name*/, std::shared_ptr<Scene>>;

	SceneManager::SceneManager()
	{
		m_nameCurrScene = "";
		m_pCurrScene = nullptr;
	}

	SceneManager::~SceneManager()
	{
	}

	std::string SceneManager::GetBasePath()
	{
		return m_basePath;
	}

	void SceneManager::SetBasePath(std::string basePath)
	{
		m_basePath = basePath;
	}

	std::shared_ptr<Scene> SceneManager::CreateNewScene(std::string newSceneName)
	{
		std::shared_ptr<Scene> pNewScene(new Scene());
		iSceneSerializer* pSceneSerializer = SceneSerializerFactory::CreateSceneSerializer(newSceneName);

		pSceneSerializer->SerializeScene(m_basePath + newSceneName, *(pNewScene));

		return pNewScene;
	}

	std::shared_ptr<Scene> SceneManager::LoadScene(std::string newSceneName)
	{
		// Scene not loaded yet
		std::shared_ptr<Scene> pNewScene(new Scene());
		iSceneSerializer* pSceneSerializer = SceneSerializerFactory::CreateSceneSerializer(newSceneName);

		pSceneSerializer->DeserializeScene(m_basePath + newSceneName, *(pNewScene));

		return pNewScene;
	}

	void SceneManager::SaveScene(std::string sceneName)
	{
		std::shared_ptr<Scene> pScene = GetScene(sceneName);
		iSceneSerializer* pSceneSerializer = SceneSerializerFactory::CreateSceneSerializer(sceneName);

		pSceneSerializer->SerializeScene(m_basePath + sceneName, *(pScene));

		return;
	}

	void SceneManager::ChangeScene(std::string newSceneName, bool reload)
	{
		std::shared_ptr<Scene> pNewScene = GetScene(newSceneName);
		itScenes it = m_mapScenes.find(newSceneName);

		if (pNewScene && reload)
		{
			// Scene already loaded, so we mark to deletion
			m_pScenesToDelete.push_back(pNewScene);
			pNewScene = nullptr;
		}
		
		if (!pNewScene)
		{
			pNewScene = LoadScene(newSceneName);
			m_mapScenes[newSceneName] = pNewScene;
		}

		m_nameCurrScene = newSceneName;
		m_pCurrScene = pNewScene;

		m_TriggerSceneChange(newSceneName, pNewScene);
	}

	std::shared_ptr<Scene> SceneManager::GetCurrentScene()
	{
		return m_pCurrScene;
	}

	std::string SceneManager::GetCurrentSceneName()
	{
		return m_nameCurrScene;
	}

	std::shared_ptr<Scene> SceneManager::GetScene(std::string sceneName)
	{
		std::shared_ptr<Scene> pScene = nullptr;
		itScenes it = m_mapScenes.find(sceneName);
		if (it == m_mapScenes.end() || !it->second)
		{
			return nullptr;
		}
		else
		{
			pScene = it->second;
			return pScene;
		}
	}

	void SceneManager::m_TriggerSceneChange(std::string newSceneName, std::shared_ptr<Scene> pNewScene)
	{
		std::shared_ptr<iEventBus<eSceneEvents, SceneChangeEvent>> pEventBus = EventBusLocator<eSceneEvents, SceneChangeEvent>::Get();

		SceneChangeEvent sceneEvent = SceneChangeEvent();
		sceneEvent.newSceneName = newSceneName;
		sceneEvent.pNewScene = pNewScene;
		pEventBus->Publish(sceneEvent);

		return;
	}
}
