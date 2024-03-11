#include "pch.h"

#include "EntitySystem.h"

#include "Engine/Events/EventsFacade.h"

#include <algorithm>

namespace MyEngine
{
	void EntitySystem::Start(std::shared_ptr<Scene> pScene)
	{
		SetSystemMask(pScene);

		SUBSCRIBE_ENTITY_ADD_EVENT(std::bind(&EntitySystem::OnEntityAdd, this, std::placeholders::_1));
		SUBSCRIBE_ENTITY_REMOVE_EVENT(std::bind(&EntitySystem::OnEntityRemove, this, std::placeholders::_1));

		// Add all entities already on the scene
		for (Entity entityId : pScene->GetEntities())
		{
			EntityMask mask = pScene->GetMask(entityId);

			if (pScene->HasComponents(entityId, m_systemMask))
			{
				m_vecEntities.push_back(entityId);
			}
		}
	}

	void EntitySystem::End(std::shared_ptr<Scene> pScene)
	{
		UNSUBSCRIBE_ENTITY_ADD_EVENT(std::bind(&EntitySystem::OnEntityAdd, this, std::placeholders::_1));
		UNSUBSCRIBE_ENTITY_REMOVE_EVENT(std::bind(&EntitySystem::OnEntityRemove, this, std::placeholders::_1));
	}

	void EntitySystem::OnEntityAdd(const EntityAddEvent& event)
	{
		if ((event.mask & m_systemMask) == m_systemMask)
		{
			m_vecEntities.push_back(event.entityId);
		}
	}

	void EntitySystem::OnEntityRemove(const EntityRemoveEvent& event)
	{
		if (event.mask == m_systemMask)
		{
			m_vecEntities.erase(std::remove(m_vecEntities.begin(), m_vecEntities.end(), event.entityId), m_vecEntities.end());
		}
	}
}
