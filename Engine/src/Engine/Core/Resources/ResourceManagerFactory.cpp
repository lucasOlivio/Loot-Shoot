#include "pch.h"

#include "ResourceManagerFactory.h"

#include "Engine/Core/Resources/Materials/MaterialManager.h"
#include "Engine/Core/Resources/Meshes/MeshManager.h"

namespace MyEngine
{
    std::shared_ptr<iResourceManager> ResourceManagerFactory::CreateResManager(const eResourceTypes& resType)
    {
        switch (resType)
        {
        case eResourceTypes::MATERIAL:
            return MaterialManager::GetInstance();
        case eResourceTypes::MESH:
            return MeshManager::GetInstance();
        default:
            LOG_ERROR("No manager for resource type: " + std::to_string(resType));
            break;
        }

        return nullptr;
    }
}
