#include "pch.h"

#include "CollisionSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Physics/Components/PhysicsLocator.h"

#include "Engine/Events/EventsFacade.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/CollisionsUtils.h"
#include "Engine/Utils/Math.h"

namespace MyEngine
{
    void CollisionSystem::Init()
    {
    }

    void CollisionSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void CollisionSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        // Clear frame collisions
        CollisionsUtils::CurrentFrameCollisions().clear();
        CollisionsUtils::CurrentFrameParticleCollisions().clear();

        std::shared_ptr<NarrowPhaseTestsComponent> pTests = PhysicsLocator::GetNarrowPhaseTests();

        // The first layer is the grouping of objects to test
        for (int group = 0; group < pTests->activeEntitiesToTest.size(); group++)
        {
            std::vector<Entity>& activeGroup = pTests->activeEntitiesToTest[group];
            std::vector<Entity>& passiveGroup = pTests->passiveEntitiesToTest[group];
            std::vector<Entity>& staticGroup = pTests->staticEntitiesToTest[group];

            for (int i = 0; i < activeGroup.size(); i++)
            {
                Entity entityId = activeGroup[i];
                TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
                RigidBodyComponent& rigidBody = pScene->Get<RigidBodyComponent>(entityId);

                m_CheckSphereOverlaps(pScene, entityId, transform, rigidBody, i,
                                        activeGroup,
                                        passiveGroup,
                                        staticGroup);
            }
        }
    }

    void CollisionSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void CollisionSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void CollisionSystem::Shutdown()
    {
    }

    bool CollisionSystem::m_RegisterFrameCollision(const sCollisionData& collData)
    {
        bool isNewCollision = CollisionsUtils::CurrentFrameCollisions().insert(collData).second;

        return isNewCollision;
    }

    void CollisionSystem::m_TriggerCollision(const sCollisionData& collData)
    {
        bool isNewCollision = m_RegisterFrameCollision(collData);
        if (!isNewCollision)
        {
            return;
        }

        RigidBodyCollisionEvent collEvent = RigidBodyCollisionEvent();
        collEvent.collisionData = collData;
        PUBLISH_RIGID_COLLISION_EVENT(collEvent);
    }

    void CollisionSystem::m_CheckSphereOverlaps(std::shared_ptr<Scene> pScene,
        Entity entityIdA,
        TransformComponent& transformA,
        RigidBodyComponent& sphereA,
        const int index,
        const std::vector<Entity>& activeEntities,
        const std::vector<Entity>& passiveEntities,
        const std::vector<Entity>& staticEntities)
    {
        // Start from one entity ahead so we dont test repeated
        for (int i = index + 1; i < activeEntities.size(); i++)
        {
            Entity entityIdB = activeEntities[i];
            bool isCollision = m_CheckSphereEntityOverlap(pScene, entityIdA,
                transformA,
                sphereA,
                entityIdB);
        }

        for (int i = 0; i < staticEntities.size(); i++)
        {
            Entity entityIdB = activeEntities[i];
            bool isCollision = m_CheckSphereEntityOverlap(pScene, entityIdA,
                transformA,
                sphereA,
                entityIdB);
        }
    }

    bool CollisionSystem::m_CheckSphereEntityOverlap(std::shared_ptr<Scene> pScene,
                                                    Entity entityIdA,
                                                    TransformComponent& transformA,
                                                    RigidBodyComponent& sphereA,
                                                    Entity entityIdB)
    {
        MovementComponent& movementA = pScene->Get<MovementComponent>(entityIdA);

        TransformComponent& transformB = pScene->Get<TransformComponent>(entityIdB);
        RigidBodyComponent& rigidBodyB = pScene->Get<RigidBodyComponent>(entityIdB);

        // Check if is colliding with entityB
        bool isCollision = false;
        
        RigidBodyComponent& sphereB = pScene->Get<RigidBodyComponent>(entityIdB);
        isCollision = CollisionsUtils::SphereSphere_Overlap(sphereA.radius,
            transformA.worldPosition,
            sphereB.radius,
            transformB.worldPosition);

        if (!isCollision)
        {
            return false;
        }

        // Is colliding, so send collision event
        sCollisionData collData = sCollisionData();
        collData.entityA = entityIdA;
        collData.entityB = entityIdB;
        collData.collisionNormalA = CollisionsUtils::SphereSphere_Normal(transformA.worldPosition,
                                                                        transformB.worldPosition);
        collData.collisionNormalB = -collData.collisionNormalA;
        collData.contactPoint = CollisionsUtils::SphereSphere_CollisionPoint(sphereA.radius,
                                                                            transformA.worldPosition,
                                                                            collData.collisionNormalA,
                                                                            sphereB.radius,
                                                                            transformB.worldPosition);
        collData.velocityAtCollisionA = movementA.velocity;

        collData.pScene = pScene;

        bool hasMovement = false;
        MovementComponent& movementB = pScene->Get<MovementComponent>(entityIdB, hasMovement);
        if (hasMovement)
        {
            collData.velocityAtCollisionB = movementB.velocity;
        }

        m_TriggerCollision(collData);

        return isCollision;
    }
}
