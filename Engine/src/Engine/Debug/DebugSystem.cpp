#include "pch.h"

#include "DebugSystem.h"

#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Meshes/Mesh.h"

#include "Engine/Debug/Components/DebugLocator.h"

namespace MyEngine
{
    void DebugSystem::Init()
    {
        std::shared_ptr<ConfigPathComponent> pConfigPath = CoreLocator::GetConfigPath();
        std::shared_ptr<iResourceManager> pMeshManager = ResourceManagerFactory::GetOrCreate(eResourceTypes::MESH);

        std::shared_ptr<DebugSquareComponent> pSquare = DebugLocator::GetSquare();
        std::shared_ptr<DebugSphereComponent> pSphere = DebugLocator::GetSphere();

        pConfigPath->LockRead();
        size_t indexSquare = pMeshManager->LoadResource(pConfigPath->pathDebugSquare);
        size_t indexSphere = pMeshManager->LoadResource(pConfigPath->pathDebugSquare);
        pConfigPath->UnlockRead();

        pSquare->LockWrite();
        pSquare->pMesh = std::static_pointer_cast<sMeshInfo>(pMeshManager->GetResource(indexSquare));
        pSquare->UnlockWrite();

        pSphere->LockWrite();
        pSphere->pMesh = std::static_pointer_cast<sMeshInfo>(pMeshManager->GetResource(indexSphere));
        pSphere->UnlockWrite();
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
