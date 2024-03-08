#pragma once

#include "Engine/ECS/Base.h"

#include "Scene.h"

namespace MyEngine
{
    // Iterator to facilitate to go only through the needed components
    // Ref: https://www.david-colson.com/2020/02/09/making-a-simple-ecs.html
    template<typename... ComponentTypes>
    class SceneView
    {
    public:
        SceneView(Scene& scene) : m_scene(scene)
        {
            if (sizeof...(ComponentTypes) == 0)
            {
                // Skip checking components and iterate over all entities
                m_all = true;
            }
            else
            {
                // Unpack the template parameters into an initializer list
                int componentIds[] = { 0, scene.GetComponentType<ComponentTypes>() ... };
                for (int i = 1; i < (sizeof...(ComponentTypes) + 1); i++)
                    m_mask.set(componentIds[i]);
            }
        }

        class Iterator
        {
        public:
            Iterator(Scene& scene, size_t index, EntityMask mask, bool all)
                : m_scene(scene), m_index(index), m_mask(mask), m_all(all)
            {}

            Entity operator*() const
            {
                return m_scene.GetEntity(m_index);
            }

            bool operator==(const Iterator& other) const
            {
                return m_index == other.m_index || m_index == m_scene.GetEntityCount();
            }

            bool operator!=(const Iterator& other) const
            {
                return m_index != other.m_index && m_index != m_scene.GetEntityCount();
            }

            Iterator& operator++()
            {
                // Next entity with correct mask
                do
                {
                    m_index++;
                } while (m_index < m_scene.GetEntityCount() &&
                    !(m_all || m_scene.HasComponents(m_scene.GetEntity(m_index), m_mask)));

                return *this;
            }

        private:
            size_t m_index;
            Scene& m_scene;
            EntityMask m_mask;
            bool m_all{ false };
        };

        const Iterator begin() const
        {
            // Index 0 is invalid entity
            size_t firstIndex = 0;
            // Find first entity with correct mask
            while (firstIndex != m_scene.GetEntityCount() &&
                !(m_all || m_scene.HasComponents(m_scene.GetEntity(firstIndex), m_mask)))
            {
                firstIndex++;
            }

            return Iterator(m_scene, firstIndex, m_mask, m_all);
        }

        const Iterator end() const
        {
            // Give an iterator to the end of this view 
            return Iterator(m_scene, m_scene.GetEntityCount(), m_mask, m_all);
        }

    protected:
        Scene& m_scene;
        EntityMask m_mask;
        bool m_all{ false };
    };
}
