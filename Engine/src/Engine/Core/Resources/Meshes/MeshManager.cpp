#include "pch.h"

#include "MeshManager.h"

#include "Engine/Core/Resources/Meshes/Mesh.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Shaders/ShaderManager.h"

#include "Engine/Assimp/cAssimpHelper.h"

#include "Engine/Graphics/opengl.h"

namespace MyEngine
{
    void MeshManager::SetBasePath(const std::string& basePath)
    {
        m_basePath = basePath;
        return;
    }

    size_t MeshManager::LoadResource(const std::string& name)
    {
        // Load the model from file
        // Checking if the info already exist in our map
        if (m_mapMeshes.find(name) != m_mapMeshes.end())
        {
            // Info is already loaded
            return m_mapMeshes.at(name)->index;
        }

        std::shared_ptr<sMeshInfo> pMesh(new sMeshInfo());
        pMesh->name = name;

        bool isLoaded = m_LoadMeshData(name, pMesh);
        assert(isLoaded);

        m_LoadVAOData(pMesh);

        // Store the draw information into cache
        m_mapMeshes[pMesh->name] = pMesh;

        pMesh->index = m_vecMeshes.size();
        m_vecMeshes.push_back(pMesh);

        return pMesh->index;
    }

    std::shared_ptr<sMeshInfo> MeshManager::LoadParticles(const std::string& name, 
												          std::vector<ParticleProps>& particles, 
												          std::vector<int>& freelist,
												          uint& particleSSBO0,
												          uint& particleSSBO1)
    {
        std::shared_ptr<sMeshInfo> pMesh(new sMeshInfo());
        pMesh->name = name;

        bool isLoaded = m_LoadMeshData(name, pMesh);
        assert(isLoaded);

        m_LoadParticleVAOData(pMesh, particles, freelist, particleSSBO0, particleSSBO1);

        return pMesh;
    }

    void MeshManager::DeleteResource(const std::string& name)
    {
        std::map<std::string, std::shared_ptr<iResource>>::iterator it = m_mapMeshes.find(name);

        assert(it != m_mapMeshes.end());

        std::shared_ptr<sMeshInfo> pMesh = std::static_pointer_cast<sMeshInfo>(it->second);
        glDeleteBuffers(1, &(pMesh->vertexBufferID));

        m_mapMeshes.erase(name);
        m_vecMeshes.erase(m_vecMeshes.begin() + pMesh->index);

        return;
    }

    void MeshManager::DeleteResource(const size_t& index)
    {
        assert(index < m_vecMeshes.size());

        std::shared_ptr<sMeshInfo> pMesh = std::static_pointer_cast<sMeshInfo>(m_vecMeshes.at(index));
        glDeleteBuffers(1, &(pMesh->vertexBufferID));

        m_mapMeshes.erase(pMesh->name);
        m_vecMeshes.erase(m_vecMeshes.begin() + pMesh->index);

        return;
    }

    void MeshManager::ClearResources()
    {
        for (size_t i = 0; i < m_vecMeshes.size();)
        {
            DeleteResource(i);
        }
    }

    std::shared_ptr<iResource> MeshManager::GetResource(size_t index)
    {
        assert(index < m_vecMeshes.size());

        return m_vecMeshes.at(index);
    }

    std::shared_ptr<iResource> MeshManager::GetResource(const std::string& name)
    {
        std::map<std::string, std::shared_ptr<iResource>>::iterator it = m_mapMeshes.find(name);

        assert(it != m_mapMeshes.end());

        return it->second;
    }

    std::vector<std::shared_ptr<iResource>>& MeshManager::GetResources()
    {
        return m_vecMeshes;
    }

    void MeshManager::ActivateResource(const std::string& name)
    {
        std::shared_ptr<sMeshInfo> pMesh = std::static_pointer_cast<sMeshInfo>(GetResource(name));
        glBindVertexArray(pMesh->VAO_ID);
        m_currMesh = name;
    }

    void MeshManager::DeactivateResource()
    {
        m_currMesh = "";
        glBindVertexArray(0);
    }

    std::shared_ptr<iResource> MeshManager::GetActiveResource()
    {
        return GetResource(m_currMesh);
    }

    bool MeshManager::m_LoadMeshData(std::string theFileName, std::shared_ptr<sMeshInfo> pMesh)
    {
        cFileLoader helper = cFileLoader();
        cFileLoader::sPostProcessFlags loadFlags;

        std::string fileFullPath = m_basePath + theFileName;

        bool isMeshLoaded = helper.Load3DModelFile(fileFullPath, loadFlags, pMesh);
        if (!isMeshLoaded)
        {
            std::string errorMsg = "Error: " + std::string(helper.getLastError().c_str());
            LOG_ERROR(errorMsg);
            return false;
        }

        return true;
    }

    void MeshManager::m_LoadVAOData(std::shared_ptr<sMeshInfo> pMesh)
    {
        // Create a VAO (Vertex Array Object), which will 
        //	keep track of all the 'state' needed to draw 
        //	from this buffer...

        // Ask OpenGL for a new buffer ID...
        glGenVertexArrays(1, &(pMesh->VAO_ID));
        // "Bind" this buffer:
        // - aka "make this the 'current' VAO buffer
        glBindVertexArray(pMesh->VAO_ID);

        // Now ANY state that is related to vertex or index buffer
        //	and vertex attribute layout, is stored in the 'state' 
        //	of the VAO... 
        glGenBuffers(1, &(pMesh->vertexBufferID));

        glBindBuffer(GL_ARRAY_BUFFER, pMesh->vertexBufferID);

        glBufferData(GL_ARRAY_BUFFER,
            sizeof(sVertex) * pMesh->numberOfVertices,
            pMesh->pVertices,
            GL_STATIC_DRAW);

        // Copy the index buffer into the video card, 
        // too create an index buffer.
        glGenBuffers(1, &(pMesh->indexBufferID));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->indexBufferID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(uint) * pMesh->numberOfIndices,
            pMesh->pIndices,
            GL_STATIC_DRAW);

        std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        // Set the vertex attributes.
        GLint vpos_location = pShader->GetAL("vPos");
        GLint vcol_location = pShader->GetAL("vCol");
        GLint vNormal_location = pShader->GetAL("vNormal");
        GLint vTextureCoords_location = pShader->GetAL("vTextureCoords");

        // Set the vertex attributes for this shader
        // vPos
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex),
            (void*)offsetof(sVertex, x));
        // vCol 
        glEnableVertexAttribArray(vcol_location);
        glVertexAttribPointer(vcol_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex),
            (void*)offsetof(sVertex, r));

        glEnableVertexAttribArray(vNormal_location);
        glVertexAttribPointer(vNormal_location, 4,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex),
            (void*)offsetof(sVertex, nx));

        glEnableVertexAttribArray(vTextureCoords_location);
        glVertexAttribPointer(vTextureCoords_location, 2,
            GL_FLOAT, GL_FALSE,
            sizeof(sVertex),
            (void*)offsetof(sVertex, u));

        // Now that all the parts are set up, set the VAO to zero
        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        glDisableVertexAttribArray(vpos_location);
        glDisableVertexAttribArray(vcol_location);

        glDisableVertexAttribArray(vNormal_location);

        glDisableVertexAttribArray(vTextureCoords_location);
    }

    void MeshManager::m_LoadParticleVAOData(std::shared_ptr<sMeshInfo> pMesh, 
								            std::vector<ParticleProps>& particles,
								            std::vector<int>& freelist,
								            uint& particleSSBO0,
								            uint& particleSSBO1)
    {
        std::shared_ptr<ShaderManager> pShader = ResourceManagerFactory::GetOrCreate<ShaderManager>(eResourceTypes::SHADER);

        // Define vertex attribute pointers
        GLint vpos_location = pShader->GetAL("vPos");
        GLint vuv_location = pShader->GetAL("vUV");
        GLint instanceLifeTime_location = pShader->GetAL("instanceLifeTime");
        GLint instanceTransform_location = pShader->GetAL("instanceTransform");
        
        // Generate a new VAO
        glGenVertexArrays(1, &(pMesh->VAO_ID));
        glBindVertexArray(pMesh->VAO_ID);

        // Now ANY state that is related to vertex or index buffer
        //	and vertex attribute layout, is stored in the 'state' 
        //	of the VAO... 
        glGenBuffers(1, &(pMesh->vertexBufferID));
        glBindBuffer(GL_ARRAY_BUFFER, pMesh->vertexBufferID);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(sVertex) * pMesh->numberOfVertices,
            pMesh->pVertices,
            GL_STATIC_DRAW);

        // Enable and set vertex attribute pointers
        glEnableVertexAttribArray(vpos_location);
        glVertexAttribPointer(vpos_location, 4, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, x));
        glEnableVertexAttribArray(vuv_location);
        glVertexAttribPointer(vuv_location, 2, GL_FLOAT, GL_FALSE, sizeof(sVertex), (void*)offsetof(sVertex, u));

        // Copy the index buffer into the video card, 
        // too create an index buffer.
        glGenBuffers(1, &(pMesh->indexBufferID));
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, pMesh->indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(uint) * pMesh->numberOfIndices,
            pMesh->pIndices,
            GL_STATIC_DRAW);

        //Create SSBO
        ParticleProps* arrayParticles = &particles[0];
        glGenBuffers(1, &particleSSBO0);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleSSBO0);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO0);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(ParticleProps) * particles.size(), arrayParticles, GL_DYNAMIC_DRAW);

        int* arrayFreelist = &freelist[0];
        glGenBuffers(1, &particleSSBO1);
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particleSSBO1);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleSSBO1);
        glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(int) * (particles.size() + 1), arrayFreelist, GL_DYNAMIC_DRAW);

        // Unbind VAO
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
