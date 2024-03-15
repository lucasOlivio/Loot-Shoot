#pragma once

#include <memory>
#include <string>
#include <vector>

namespace MyEngine
{
    enum eResourceTypes
    {
        UNKNOWN_RESOURCE = 0,
        MATERIAL,
        MESH,
        SHADER,
        TEXTURE
    };

    struct iResource
    {
        size_t index = 0;
        std::string name = "";
    };

    class iResourceManager
    {
    public:
        virtual void SetBasePath(const std::string& basePath) = 0;
        virtual size_t LoadResource(const std::string& name) = 0;

        virtual void DeleteResource(const std::string& name) = 0;
        virtual void DeleteResource(const size_t& index) = 0;
        virtual void ClearResources() = 0;

        virtual std::shared_ptr<iResource> GetResource(size_t index) = 0;
        virtual std::shared_ptr<iResource> GetResource(const std::string& name) = 0;

        virtual std::vector<std::shared_ptr<iResource>>& GetResources() = 0;

        virtual void ActivateResource(const size_t& index) = 0;
        virtual void ActivateResource(const std::string& name) = 0;
        virtual void DeactivateResource() = 0;
        virtual std::shared_ptr<iResource> GetActiveResource() = 0;
    };
}
