#pragma once

#include "Event.hpp"
#include "Engine/ECS/Scene/Scene.h"

namespace MyEngine
{
	enum eSceneEvents
	{
		ENTITY_ADD,
		ENTITY_REMOVE
	};

	class EntityAddEvent : public Event<eSceneEvents>
	{
	public:
		EntityAddEvent() :
			Event<eSceneEvents>(eSceneEvents::ENTITY_ADD, "EntityAdd") {};
		virtual ~EntityAddEvent() {};

		Entity entityId;
		EntityMask mask;
	};

	class EntityRemoveEvent : public Event<eSceneEvents>
	{
	public:
		EntityRemoveEvent() :
			Event<eSceneEvents>(eSceneEvents::ENTITY_REMOVE, "EntityRemove") {};
		virtual ~EntityRemoveEvent() {};

		Entity entityId;
		EntityMask mask;
	};
}
