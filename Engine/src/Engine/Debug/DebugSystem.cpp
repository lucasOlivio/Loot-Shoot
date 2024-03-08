#include "pch.h"

#include "DebugSystem.h"

#include "Engine/Core/Components/CoreLocator.h"

#include "Engine/Debug/Components/DebugLocator.h"

#include "Engine/Graphics/VAO/VAOManagerLocator.h"

namespace MyEngine
{
    void DebugSystem::Init()
    {
        std::shared_ptr<ConfigPathComponent> pConfigPath = CoreLocator::GetConfigPath();
        std::shared_ptr<iVAOManager> pVAOManager = VAOManagerLocator::Get();

        std::shared_ptr<DebugSquareComponent> pSquare = DebugLocator::GetSquare();
        std::shared_ptr<DebugSphereComponent> pSphere = DebugLocator::GetSphere();

        pSquare->pMesh = pVAOManager->LoadModelIntoVAO(pConfigPath->pathDebugSquare, true);
        pSphere->pMesh = pVAOManager->LoadModelIntoVAO(pConfigPath->pathDebugSphere, true);
    }

    void DebugSystem::Start(std::shared_ptr<Scene> pScene)
    {
    }

    void DebugSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
    {
    }

    void DebugSystem::Render(std::shared_ptr<Scene> pScene)
    {
    }

    void DebugSystem::End(std::shared_ptr<Scene> pScene)
    {
    }

    void DebugSystem::Shutdown()
    {
    }
}
