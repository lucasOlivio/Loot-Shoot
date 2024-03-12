#pragma once

#include "Engine/ThreadSafe.h"

#include "Engine/Core/Resources/iResourceManager.h"

#include "Engine/Singleton.hpp"

#include <map>

namespace MyEngine
{
	class MaterialManager : public Singleton<MaterialManager>,
							public iResourceManager,
							public ThreadSafe
	{
		friend class Singleton<MaterialManager>;
	public:
		virtual void SetBasePath(const std::string& basePath);
		virtual size_t LoadResource(const std::string& name);

		virtual void DeleteResource(const std::string& name);
		virtual void DeleteResource(const size_t& index);
		virtual void ClearResources();

		virtual std::shared_ptr<iResource> GetResource(size_t index);
		virtual std::shared_ptr<iResource> GetResource(const std::string& name);
		virtual std::vector<std::shared_ptr<iResource>>& GetResources();

		virtual void ActivateResource(const std::string& name);
		virtual void DeactivateResource();
		virtual std::shared_ptr<iResource> GetActiveResource();

	private:
		MaterialManager() {};

		std::map<std::string, std::shared_ptr<iResource>> m_mapMaterials;
		std::vector<std::shared_ptr<iResource>> m_vecMaterials;

		std::shared_ptr<iResource> m_defaultMaterial;

		// Textures and variables in the uniforms and units at the moment
		std::string m_currMaterial;
	};
}