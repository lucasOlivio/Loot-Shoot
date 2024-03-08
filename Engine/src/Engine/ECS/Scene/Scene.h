#pragma once

#include "Engine/ECS/EntityManager.h"
#include "Engine/ECS/ComponentPool.h"

#include "Engine/Utils/BiMap.hpp"
#include "Engine/Utils/Log.h"

#include <set>

namespace MyEngine
{
    class Engine;

    class Scene 
    {
    public:
        Scene();
        ~Scene();

        // Create an entity using the EntityManager
        // addDefault = Add default components (Transform) to it
        Entity CreateEntity(bool addDefault = false);

        // Flag entity to be deleted later on a safer time
        void RemoveEntity(Entity entityId);

        // Unique ID for each component type
        template <class T>
        ComponentType GetComponentType()
        {
            static int componentType = m_componentCounter++;
            return componentType;
        }

        // Add new component to entity and return the instance
        template <typename T>
        T& AddComponent(Entity entityId)
        {
            ComponentType componentType = GetComponentType<T>();

            if (m_componentPools.find(componentType) == m_componentPools.end())
            {
                // New component type, make a new pool
                m_componentPools[componentType].reset(new ComponentPool(sizeof(T)));
                m_componentMaps[componentType].reset(new BiMap<Entity, ComponentId>());
            }

            bool hasComponent = false;
            T& component = Get<T>(entityId, hasComponent);
            if (hasComponent)
            {
                // Entity already have component, so just return it
                return component;
            }

            // Looks up the component id and mem location in the pool, 
            // and initializes it with placement new
            ComponentId componentId;
            void* mem = m_componentPools[componentType]->Add(componentId);
            T& newComponent = *(new (mem) T());

            // Set the relation between the component id and the entity id
            m_componentMaps[componentType]->Add(entityId, componentId);

            // Add component to the entity mask and return the created component
            m_pEntityManager->SetComponent(entityId, componentType);

            return newComponent;
        }

        // Get an entity component using the BiMap and ComponentPool
        // We assume that the systems will only try to get the entity component that it knows that it have
        template<typename T>
        T& Get(Entity entityId, bool& hasComponent)
        {
            ComponentType componentType = GetComponentType<T>();
            ComponentId componentId;

            if (m_componentPools.find(componentType) == m_componentPools.end())
            {
                // New component type, make a new pool
                m_componentPools[componentType].reset(new ComponentPool(sizeof(T)));
                m_componentMaps[componentType].reset(new BiMap<Entity, ComponentId>());
            }

            std::unique_ptr<ComponentPool>& pComponentPool = m_componentPools[componentType];
            std::unique_ptr<BiMap<Entity, ComponentId>>& pComponentMap = m_componentMaps[componentType];

            hasComponent = pComponentMap->GetByKey(entityId, componentId);

            if (!hasComponent)
            {
                // Index 0 been a empty component not related to any entity
                return *(std::shared_ptr<T>(new T()));
            }

            T& component = *(static_cast<T*>(pComponentPool->Get(componentId)));

            return component;
        }

        template<typename T>
        T& Get(Entity entityId)
        {
            bool temp = false;
            return Get<T>(entityId, temp);
        }

        // Remove a component from an entity and all the mappings
        void RemoveComponent(Entity entityId, ComponentType componentType);

        size_t GetComponentCount() const;

        // TODO: Use a mediator design here to avoid this repetition from this class?
        // Entity manager views
        size_t GetEntityCount() const;
        
        EntityMask GetMask(const Entity& entityId) const;

        bool HasComponent(const Entity& entityId, const ComponentType& componentType) const;
        bool HasComponents(const Entity& entityId, const EntityMask& componentMask) const;

        const std::vector<Entity>& GetEntities() const;

        // Return entity Id accordingly to index in live entities vector
        Entity GetEntity(const size_t& index) const;

    private:
        std::unique_ptr<EntityManager> m_pEntityManager;
        std::unordered_map<ComponentType, std::unique_ptr<BiMap<Entity, ComponentId>>> m_componentMaps;
        std::unordered_map<ComponentType, std::unique_ptr<ComponentPool>> m_componentPools;
        
        int m_componentCounter;

        struct CompToDestroy
        {
            ComponentType componentType;
            Entity entityId;

            // Overloads for <set> usage
            bool operator==(const CompToDestroy& other) const
            {
                return entityId == other.entityId && componentType == other.componentType;
            }

            bool operator<(const CompToDestroy& other) const
            {
                return entityId < other.entityId;
            }
        };

        // Using set to avoid setting the same entity/component to be destroied on multithreading
        std::set<Entity> m_entitiesToDestroy;
        std::set<CompToDestroy> m_componentsToDestroy;

        // Really removes and destroy all pending entities and its components
        void m_DestroyEntities();
        void m_DestroyComponents();

        // Allow engine to call the destroy entities member function when needed
        friend class Engine;
    };
}