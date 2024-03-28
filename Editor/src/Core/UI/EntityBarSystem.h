#pragma once

#include "Engine/ECS/System/iSystem.h"

#include "Engine/Events/SceneEvent.h"

namespace MyEngine
{
	// Side bar with selected entity and components UI
	class EntityBarSystem : public iSystem
	{
	public:
		EntityBarSystem() = default;
		virtual ~EntityBarSystem() { };

		virtual std::string SystemName() { return "EntityBarSystem"; };

		virtual void Init();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void Update(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void Render(std::shared_ptr<Scene> pScene, float deltaTime);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void Shutdown();

	private:
		void m_CreateEntity(Scene& scene);
		void m_CreateEntity(Scene& scene, Entity entityId);

		void m_RemoveEntity(Scene& scene, Entity entityId);

		void m_AddComponentsList(Scene& scene, Entity entityId);

		template <typename T>
		void m_AddComponent(Scene& scene, Entity entityId)
		{
			scene.AddComponent<T>(entityId);
		}
	};
}
