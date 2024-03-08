#include "pch.h"

#include "PhysicsLocator.h"

namespace MyEngine
{
    std::shared_ptr<NarrowPhaseTestsComponent> PhysicsLocator::m_pNarrowPhaseTests(new NarrowPhaseTestsComponent());
    std::shared_ptr<GridBroadphaseComponent> PhysicsLocator::m_pGridBroadphase(new GridBroadphaseComponent());
    std::shared_ptr<FrameCollisionComponent> PhysicsLocator::m_pFrameCollision(new FrameCollisionComponent());

    std::shared_ptr<NarrowPhaseTestsComponent> PhysicsLocator::GetNarrowPhaseTests()
    {
        return m_pNarrowPhaseTests;
    }

    std::shared_ptr<GridBroadphaseComponent> PhysicsLocator::GetGridBroadphase()
    {
        return m_pGridBroadphase;
    }

    std::shared_ptr<FrameCollisionComponent> PhysicsLocator::GetFrameCollision()
    {
        return m_pFrameCollision;
    }

    void PhysicsLocator::SetNarrowPhaseTests(std::shared_ptr<NarrowPhaseTestsComponent> pNarrowPhaseTests)
    {
        m_pNarrowPhaseTests = pNarrowPhaseTests;
    }

    void PhysicsLocator::SetGridBroadphase(std::shared_ptr<GridBroadphaseComponent> pGridBroadphase)
    {
        m_pGridBroadphase = pGridBroadphase;
    }

    void PhysicsLocator::SetFrameCollision(std::shared_ptr<FrameCollisionComponent> pFrameCollision)
    {
        m_pFrameCollision = pFrameCollision;
    }
}
