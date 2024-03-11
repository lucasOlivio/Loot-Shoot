#pragma once

#include "Engine/ECS/Base.h"

#include "Engine/Events/SceneEvent.h"

#include <vector>

namespace MyEngine
{
	// Base for all other systems, to control the entities that this system should iterate over
	class BaseSystem
	{
	public:
		BaseSystem() {};
		virtual ~BaseSystem() {};

		virtual void OnEntityAdd(const EntityAddEvent& event);

		virtual void OnEntityRemove(const EntityRemoveEvent& event);

	private:
		EntityMask m_systemMask;
		std::vector<Entity> m_vecEntities;
	};
}
