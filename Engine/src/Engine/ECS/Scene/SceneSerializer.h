#pragma once

#include "Scene.h"

namespace MyEngine {

	class SceneSerializer
	{
	public:
		SceneSerializer(std::shared_ptr<Scene> pScene);

		bool Serialize(const std::string& filepath);

		bool Deserialize(const std::string& filepath);
	private:
		std::shared_ptr<Scene> m_pScene;
	};

}
