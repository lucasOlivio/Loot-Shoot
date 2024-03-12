#pragma once

#include "iSystem.h"

#include "Engine/ThreadSafe.h"

#include "Engine/ECS/Base.h"

#include "Engine/Events/SceneEvent.h"

#include <Windows.h>
#include <vector>

namespace MyEngine
{
	// Base for all other systems that iterates over entities
	// Uses the entities events to control the entities that this system should iterate over
	class EntitySystem : public iSystem,
						 public ThreadSafe
	{
	public:
		EntitySystem();
		virtual ~EntitySystem();

		virtual void Start(std::shared_ptr<Scene> pScene);

		virtual void End(std::shared_ptr<Scene> pScene);

		virtual void OnEntityAdd(const EntityAddEvent& event);

		virtual void OnEntityRemove(const EntityRemoveEvent& event);

		// System should just set here the components that the entity MUST have to be updated by it
		virtual void SetSystemMask(std::shared_ptr<Scene> pScene) = 0;

	protected:
		EntityMask m_systemMask;
		std::vector<Entity> m_vecEntities;
	};
}
