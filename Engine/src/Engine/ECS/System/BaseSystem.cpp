#include "pch.h"

#include "BaseSystem.h"

#include <algorithm>

namespace MyEngine
{
	void BaseSystem::OnEntityAdd(const EntityAddEvent& event)
	{
		if (event.mask == m_systemMask)
		{
			m_vecEntities.push_back(event.entityId);
		}
	}

	void BaseSystem::OnEntityRemove(const EntityRemoveEvent& event)
	{
		if (event.mask == m_systemMask)
		{
			m_vecEntities.erase(std::remove(m_vecEntities.begin(), m_vecEntities.end(), event.entityId), m_vecEntities.end());
		}
	}
}
