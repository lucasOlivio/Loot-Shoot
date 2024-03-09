#include "pch.h"

#include "ResourceManagerFactory.h"

#include "Engine/Core/Resources/Materials/MaterialManager.h"

namespace MyEngine
{
    std::shared_ptr<iResourceManager> ResourceManagerFactory::CreateResManager(const eResourceTypes& resType)
    {
        switch (resType)
        {
        case eResourceTypes::MATERIAL:
            return MaterialManager::GetInstance();
        default:
            LOG_ERROR("No manager for resource type: " + std::to_string(resType));
            break;
        }

        return nullptr;
    }
}
