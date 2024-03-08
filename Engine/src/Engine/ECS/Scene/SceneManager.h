#pragma once

#include "Engine/ECS/Scene/iSceneManager.h"

#include <map>

namespace MyEngine
{
	class SceneManager : public iSceneManager
	{
	public:
		SceneManager();
		virtual ~SceneManager();

		virtual std::string GetBasePath();
		virtual void SetBasePath(std::string basePath);

		virtual std::shared_ptr<Scene> CreateNewScene(std::string newSceneName);

		virtual std::shared_ptr<Scene> LoadScene(std::string newSceneName);

		virtual void SaveScene(std::string sceneName);

		virtual void ChangeScene(std::string newSceneName, bool reload = false);

		virtual std::shared_ptr<Scene> GetCurrentScene();
		virtual std::string GetCurrentSceneName();

		virtual std::shared_ptr<Scene> GetScene(std::string sceneName);

	private:
		std::string m_basePath;

		std::string m_nameCurrScene;
		std::shared_ptr<Scene> m_pCurrScene;

		std::map<std::string /*Scene name*/, std::shared_ptr<Scene>> m_mapScenes;

		// Avoid deleting scene while somewhere else is using on reload
		std::vector<std::shared_ptr<Scene>> m_pScenesToDelete;

		void m_TriggerSceneChange(std::string newSceneName, std::shared_ptr<Scene> pNewScene);
	};
}
