#include "pch.h"

#include "DebugLocator.h"
#include "Engine/Graphics/VAO/VAOManagerLocator.h"

namespace MyEngine
{
    std::shared_ptr<DebugSquareComponent> DebugLocator::m_pDebugSquare(new DebugSquareComponent());
    std::shared_ptr<DebugSphereComponent> DebugLocator::m_pDebugSphere(new DebugSphereComponent());

    std::shared_ptr<DebugSquareComponent> DebugLocator::GetSquare()
    {
        return m_pDebugSquare;
    }

    std::shared_ptr<DebugSphereComponent> DebugLocator::GetSphere()
    {
        return m_pDebugSphere;
    }

    void DebugLocator::SetSquare(std::shared_ptr<DebugSquareComponent> pDebugSquare)
    {
        m_pDebugSquare = pDebugSquare;
    }

    void DebugLocator::SetSphere(std::shared_ptr<DebugSphereComponent> pDebugSphere)
    {
        m_pDebugSphere = pDebugSphere;
    }
}
