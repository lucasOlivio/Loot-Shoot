#pragma once

#include "Engine/Core/Resources/iResourceManager.h"
#include "Engine/Core/Resources/Meshes/Mesh.h"

#include "Engine/Singleton.hpp"

#include <map>

namespace MyEngine
{
	class MeshManager : public Singleton<MeshManager>,
		public iResourceManager
	{
		friend class Singleton<MeshManager>;
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
		MeshManager() {};

		// VAO binded to shader at moment
		std::string m_currMesh;
		std::string m_basePath;

		std::map<std::string, std::shared_ptr<iResource>> m_mapMeshes;
		std::vector<std::shared_ptr<iResource>> m_vecMeshes;

		std::shared_ptr<iResource> m_defaultMesh;

		bool m_LoadMeshData(std::string theFileName, std::shared_ptr<sMeshInfo> pMesh);
		void m_LoadVAOData(std::shared_ptr<sMeshInfo> pMesh);
	};
}