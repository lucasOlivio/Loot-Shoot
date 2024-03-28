#include "pch.h"

#include "NarrowPhaseSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Physics/Components/PhysicsLocator.h"

#include "Engine/Events/EventsFacade.h"

#include "Engine/Utils/TransformUtils.h"
#include "Engine/Utils/CollisionsUtils.h"
#include "Engine/Utils/Math.h"
#include "Engine/Utils/Random.h"

namespace MyEngine
{
    void NarrowPhaseSystem::Init()
    {
    }

    void NarrowPhaseSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void NarrowPhaseSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
        //// Clear frame collisions
        //CollisionsUtils::CurrentFrameCollisions().clear();
        //CollisionsUtils::CurrentFrameParticleCollisions().clear();

        //std::shared_ptr<NarrowPhaseTestsComponent> pTests = PhysicsLocator::GetNarrowPhaseTests();

        //// The first layer is the grouping of objects to test
        //pTests->LockRead();
        //for (int group = 0; group < pTests->activeEntitiesToTest.size(); group++)
        //{
        //    std::vector<Entity>& activeGroup = pTests->activeEntitiesToTest[group];
        //    std::vector<Entity>& passiveGroup = pTests->passiveEntitiesToTest[group];
        //    std::vector<Entity>& staticGroup = pTests->staticEntitiesToTest[group];

        //    for (int i = 0; i < activeGroup.size(); i++)
        //    {
        //        Entity entityId = activeGroup[i];
        //        TransformComponent& transform = pScene->Get<TransformComponent>(entityId);
        //        RigidBodyComponent& rigidBody = pScene->Get<RigidBodyComponent>(entityId);

        //        m_CheckSphereOverlaps(pScene, entityId, transform, rigidBody, i,
        //            activeGroup,
        //            passiveGroup,
        //            staticGroup);
        //    }
        //}
        //pTests->UnlockRead();
    }

    void NarrowPhaseSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void NarrowPhaseSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void NarrowPhaseSystem::Shutdown()
    {
    }

    bool NarrowPhaseSystem::m_RegisterFrameCollision(const sCollisionData& collData)
    {
        bool isNewCollision = CollisionsUtils::CurrentFrameCollisions().insert(collData).second;

        return isNewCollision;
    }

    void NarrowPhaseSystem::m_TriggerCollision(const sCollisionData& collData)
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

    void NarrowPhaseSystem::m_CheckSphereOverlaps(std::shared_ptr<Scene> pScene,
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

    bool NarrowPhaseSystem::m_CheckSphereEntityOverlap(std::shared_ptr<Scene> pScene,
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

        sphereA.LockRead();
        sphereB.LockRead();
        transformA.LockRead();
        transformB.LockRead();
        isCollision = CollisionsUtils::SphereSphere_Overlap(sphereA.radius,
                                                            transformA.position,
                                                            sphereB.radius,
                                                            transformB.position);
        transformB.UnlockRead();
        transformA.UnlockRead();
        sphereB.UnlockRead();
        sphereA.UnlockRead();

        if (!isCollision)
        {
            return false;
        }

        // Is colliding, so send collision event
        sCollisionData collData = sCollisionData();
        collData.entityA = entityIdA;
        collData.entityB = entityIdB;

        transformA.LockRead();
        transformB.LockRead();
        collData.collisionNormalA = CollisionsUtils::SphereSphere_Normal(transformA.position,
                                                                        transformB.position);
        transformB.UnlockRead();
        transformA.UnlockRead();

        collData.collisionNormalB = -collData.collisionNormalA;

        sphereA.LockRead();
        sphereB.LockRead();
        transformA.LockRead();
        transformB.LockRead();
        collData.contactPoint = CollisionsUtils::SphereSphere_CollisionPoint(sphereA.radius,
                                                                            transformA.position,
                                                                            collData.collisionNormalA,
                                                                            sphereB.radius,
                                                                            transformB.position);
        transformB.UnlockRead();
        transformA.UnlockRead();
        sphereB.UnlockRead();
        sphereA.UnlockRead();

        movementA.LockRead();
        collData.velocityAtCollisionA = movementA.velocity;
        movementA.UnlockRead();

        collData.pScene = pScene;

        bool hasMovement = false;
        MovementComponent& movementB = pScene->Get<MovementComponent>(entityIdB, hasMovement);
        if (hasMovement)
        {
            movementB.LockRead();
            collData.velocityAtCollisionB = movementB.velocity;
            movementB.UnlockRead();
        }

        m_TriggerCollision(collData);

        return isCollision;
    }
}
