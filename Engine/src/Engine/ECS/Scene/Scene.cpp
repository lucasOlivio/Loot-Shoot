#include "pch.h"

#include "Scene.h"

#include "Engine/ECS/Components.h"

namespace MyEngine
{
    Scene::Scene() : m_pEntityManager(new EntityManager()), m_componentCounter(0)
    {}

    Scene::~Scene()
    {}

    Entity Scene::CreateEntity(bool addDefault)
    {
        Entity entityId = m_pEntityManager->AddEntity(EntityMask());

        if (addDefault)
        {
            AddComponent<TransformComponent>(entityId);
        }

        return entityId;
    }

    Entity Scene::CreateEntity(Entity entityId)
    {
        Entity newEntityId = CreateEntity();

        EntityMask entityMask = m_pEntityManager->GetMask(entityId);
        for (int componentType = 0; componentType < entityMask.size(); componentType++)
        {
            if (!entityMask[componentType])
            {
                continue;
            }

            // Check which component it is and add to the new entity
            if (componentType == GetComponentType<TagComponent>())
            {
                bool hastag = false;
                TagComponent& tagOrig = Get<TagComponent>(entityId, hastag);

                if(hastag)
                {
                    TagComponent& tag = AddComponent<TagComponent>(newEntityId);
                    tag = Get<TagComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<TransformComponent>())
            {
                bool hastransform = false;
                TransformComponent& transformOrig = Get<TransformComponent>(entityId, hastransform);

                if (hastransform)
                {
                    TransformComponent& transform = AddComponent<TransformComponent>(newEntityId);
                    transform = Get<TransformComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<MovementComponent>())
            {
                bool hasmovement = false;
                MovementComponent& movementOrig = Get<MovementComponent>(entityId, hasmovement);

                if (hasmovement)
                {
                    MovementComponent& movement = AddComponent<MovementComponent>(newEntityId);
                    movement = Get<MovementComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<RotationComponent>())
            {
                bool hasrotation = false;
                RotationComponent& rotationOrig = Get<RotationComponent>(entityId, hasrotation);

                if (hasrotation)
                {
                    RotationComponent& rotation = AddComponent<RotationComponent>(newEntityId);
                    rotation = Get<RotationComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<ParentComponent>())
            {
                bool hasparent = false;
                ParentComponent& parentOrig = Get<ParentComponent>(entityId, hasparent);

                if (hasparent)
                {
                    ParentComponent& parent = AddComponent<ParentComponent>(newEntityId);
                    parent = Get<ParentComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<TextureComponent>())
            {
                bool hastexture = false;
                TextureComponent& textureOrig = Get<TextureComponent>(entityId, hastexture);

                if (hastexture)
                {
                    TextureComponent& texture = AddComponent<TextureComponent>(newEntityId);
                    texture = Get<TextureComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<MaterialComponent>())
            {
                bool hasmaterial = false;
                MaterialComponent& materialOrig = Get<MaterialComponent>(entityId, hasmaterial);

                if (hasmaterial)
                {
                    MaterialComponent& material = AddComponent<MaterialComponent>(newEntityId);
                    material = Get<MaterialComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<LightComponent>())
            {
                bool haslight = false;
                LightComponent& lightOrig = Get<LightComponent>(entityId, haslight);

                if (haslight)
                {
                    LightComponent& light = AddComponent<LightComponent>(newEntityId);
                    light = Get<LightComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<ModelComponent>())
            {
                bool hasmodel = false;
                ModelComponent& modelOrig = Get<ModelComponent>(entityId, hasmodel);

                if (hasmodel)
                {
                    ModelComponent& model = AddComponent<ModelComponent>(newEntityId);
                    model = Get<ModelComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<TransformAnimationComponent>())
            {
                bool hastransformAnimation = false;
                TransformAnimationComponent& transformAnimationOrig = Get<TransformAnimationComponent>(entityId, hastransformAnimation);

                if (hastransformAnimation)
                {
                    TransformAnimationComponent& transformAnimation = AddComponent<TransformAnimationComponent>(newEntityId);
                    transformAnimation = Get<TransformAnimationComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<TilingComponent>())
            {
                bool hastiling = false;
                TilingComponent& tilingOrig = Get<TilingComponent>(entityId, hastiling);

                if (hastiling)
                {
                    TilingComponent& tiling = AddComponent<TilingComponent>(newEntityId);
                    tiling = Get<TilingComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<RigidBodyComponent>())
            {
                bool hasrigidBody = false;
                RigidBodyComponent& rigidBodyOrig = Get<RigidBodyComponent>(entityId, hasrigidBody);

                if (hasrigidBody)
                {
                    RigidBodyComponent& rigidBody = AddComponent<RigidBodyComponent>(newEntityId);
                    rigidBody = Get<RigidBodyComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<MeshColliderComponent>())
            {
                bool hasmeshCollider = false;
                MeshColliderComponent& meshColliderOrig = Get<MeshColliderComponent>(entityId, hasmeshCollider);

                if (hasmeshCollider)
                {
                    MeshColliderComponent& meshCollider = AddComponent<MeshColliderComponent>(newEntityId);
                    meshCollider = Get<MeshColliderComponent>(entityId);
                }
            }
            else if (componentType == GetComponentType<SphereColliderComponent>())
            {
                bool hassphereCollider = false;
                SphereColliderComponent& sphereColliderOrig = Get<SphereColliderComponent>(entityId, hassphereCollider);

                if (hassphereCollider)
                {
                    SphereColliderComponent& sphereCollider = AddComponent<SphereColliderComponent>(newEntityId);
                    sphereCollider = Get<SphereColliderComponent>(entityId);
                }
            }
        }

        return Entity();
    }

    void Scene::RemoveEntity(Entity entityId)
    {
        // TODO: Critical section for vec componentsToDestroy
        m_entitiesToDestroy.insert(entityId);
        m_pEntityManager->SetMask(entityId, EntityMask()); // Clear mask
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
            // Go through every component the entity have to remove them
            EntityMask entityMask = m_pEntityManager->GetMask(entityId);
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
}
