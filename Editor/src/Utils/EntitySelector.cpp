#include "EntitySelector.h"

namespace MyEngine
{
    // Entity 0 should always be the empty config
    Entity EntitySelector::m_selectedEntity = 1;

    EntitySelector::EntitySelector()
    {
    }

    EntitySelector::~EntitySelector()
    {
    }

    Entity EntitySelector::GetSelectedEntity()
    {
        return m_selectedEntity;
    }

    Entity EntitySelector::NextEntity(const Scene& scene)
    {
        if (scene.GetEntityCount() == 0)
        {
            m_selectedEntity = 0;
            return m_selectedEntity;
        }

        // WARNING: Having only empty mask entities could cause infinite loop
        do
        {
            m_selectedEntity++;

            if (m_selectedEntity >= MAX_ENTITIES)
            {
                m_selectedEntity = 0;
            }
        } while (scene.GetMask(m_selectedEntity) == EntityMask() && m_selectedEntity != 0); // HACK: Entity 0 "has" singleton components

        return m_selectedEntity;
    }

    Entity EntitySelector::PrevEntity(const Scene& scene)
    {
        if (scene.GetEntityCount() == 0)
        {
            m_selectedEntity = 0;
            return m_selectedEntity;
        }

        // WARNING: Having only empty mask entities could cause infinite loop
        do
        {
            m_selectedEntity--;

            if (m_selectedEntity < 0)
            {
                m_selectedEntity = MAX_ENTITIES;
            }
        } while (scene.GetMask(m_selectedEntity) == EntityMask() && m_selectedEntity != 0); // HACK: Entity 0 "has" singleton components

        return m_selectedEntity;
    }

    void EntitySelector::SetSelectedEntity(Entity entityId)
    {
        m_selectedEntity = entityId;
    }
}
