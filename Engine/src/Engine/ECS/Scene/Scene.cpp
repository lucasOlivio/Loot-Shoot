#include "pch.h"

#include "Scene.h"

#include "Engine/Core/Components/Components.h"

#include "Engine/Events/EventsFacade.h"

namespace MyEngine
{
    Scene::Scene() : m_pEntityManager(new EntityManager()), m_componentCounter(0)
    {}

    Scene::~Scene()
    {}

    Entity Scene::CreateEntity()
    {
        Entity entityId = m_pEntityManager->AddEntity(EntityMask());

        return entityId;
    }

    void Scene::RemoveEntity(Entity entityId)
    {
        // TODO: Critical section for vec componentsToDestroy
        m_entitiesToDestroy.insert(entityId);
        m_pEntityManager->SetMask(entityId, EntityMask()); // Clear mask

        m_TriggerEntityRemoval(entityId);
    }

    void Scene::RemoveComponent(Entity entityId, ComponentType componentType)
    {
        EntityMask entityMask = m_pEntityManager->GetMask(entityId);

        if (!entityMask[componentType])
        {
            return;
        }

        // TODO: Critical section for vec componentsToDestroy
        CompToDestroy comp = CompToDestroy();
        comp.componentType = componentType;
        comp.entityId = entityId;
        m_componentsToDestroy.insert(comp);
        m_pEntityManager->UnsetComponent(entityId, componentType); // Remove component from mask
    }

    size_t Scene::GetComponentCount() const
    {
        return m_componentPools.size();
    }

    size_t Scene::GetEntityCount() const
    {
        return m_pEntityManager->Size();
    }

    EntityMask Scene::GetMask(const Entity& entityId) const
    {
        return m_pEntityManager->GetMask(entityId);
    }

    bool Scene::HasComponents(const Entity& entityId, const EntityMask& componentMask) const
    {
        return m_pEntityManager->HasComponents(entityId, componentMask);
    }

    bool Scene::HasComponent(const Entity& entityId, const ComponentType& componentType) const
    {
        return m_pEntityManager->HasComponent(entityId, componentType);
    }

    const std::vector<Entity>& Scene::GetEntities() const
    {
        return m_pEntityManager->GetEntities();
    }

    Entity Scene::GetEntity(const size_t& index) const
    {
        return m_pEntityManager->GetEntity(index);
    }

    void Scene::m_DestroyEntities()
    {
        for (Entity entityId : m_entitiesToDestroy)
        {
            EntityMask entityMask = m_pEntityManager->GetMask(entityId);

            // Go through every component the entity have to remove them
            for (int componentType = 0; componentType < entityMask.size(); componentType++)
            {
                RemoveComponent(entityId, componentType);
            }

            // Remove entity from entity manager list
            m_pEntityManager->RemoveEntity(entityId); 
        }

        m_entitiesToDestroy.clear();
    }

    void Scene::m_DestroyComponents()
    {
        for (CompToDestroy comp : m_componentsToDestroy)
        {
            std::unique_ptr<BiMap<Entity, ComponentId>>& pComponentMap = m_componentMaps[comp.componentType];
            std::unique_ptr<ComponentPool>& pComponentPool = m_componentPools[comp.componentType];

            // Component to remove and new location for moved component
            ComponentId componentId;
            bool isValid = pComponentMap->GetByKey(comp.entityId, componentId);
            if (!isValid)
            {
                continue;
            }

            // Remove the component from the pool
            ComponentId componentIdOld;
            pComponentPool->Remove(componentId, componentIdOld);

            // Remove the relation mapping
            pComponentMap->RemoveByKey(comp.entityId);

            // Remove component flag mask
            m_pEntityManager->UnsetComponent(comp.entityId, comp.componentType);

            // Get entity id that had component moved
            Entity entityIdMoved;
            isValid = pComponentMap->GetByValue(componentIdOld, entityIdMoved);
            if (!isValid)
            {
                continue;
            }

            // Update the relation for the moved component
            pComponentMap->Update(entityIdMoved, componentId);
        }

        m_componentsToDestroy.clear();
    }

    void Scene::m_TriggerEntityCreation(Entity entityId)
    {
        EntityMask entityMask = m_pEntityManager->GetMask(entityId);
        
        EntityAddEvent enttEvent = EntityAddEvent();
        enttEvent.entityId = entityId;
        enttEvent.mask = entityMask;
        PUBLISH_ENTITY_ADD_EVENT(enttEvent);
    }
    
    void Scene::m_TriggerEntityRemoval(Entity entityId)
    {
        EntityMask entityMask = m_pEntityManager->GetMask(entityId);

        EntityRemoveEvent enttEvent = EntityRemoveEvent();
        enttEvent.entityId = entityId;
        enttEvent.mask = entityMask;
        PUBLISH_ENTITY_REMOVE_EVENT(enttEvent);
    }
}
