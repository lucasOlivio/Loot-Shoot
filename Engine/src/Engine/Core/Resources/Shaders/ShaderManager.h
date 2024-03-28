#pragma once

#include "Engine/Core/Resources/iResourceManager.h"

#include "Engine/Singleton.hpp"

#include <map>

namespace MyEngine
{
	class ShaderManager : public Singleton<ShaderManager>,
							public iResourceManager
	{
		friend class Singleton<ShaderManager>;
	public:
		virtual void SetBasePath(const std::string& basePath);
		virtual size_t LoadResource(const std::string& name);
		size_t LoadComputeShader(const std::string& name);

		virtual void DeleteResource(const std::string& name);
		virtual void DeleteResource(const size_t& index);
		virtual void ClearResources();

		virtual std::shared_ptr<iResource> GetResource(size_t index);
		virtual std::shared_ptr<iResource> GetResource(const std::string& name);
		virtual std::vector<std::shared_ptr<iResource>>& GetResources();

		// No need for performance access for now
		virtual void ActivateResource(const size_t& index) {};
		virtual void ActivateResource(const std::string& name);
		virtual void DeactivateResource();
		virtual std::shared_ptr<iResource> GetActiveResource();

		// Uniform managing (Should it be separated?)
		// ----------------------------------------------

		void SetWireframe(bool isWireframe);
		// Returns -1 (just like OpenGL) if NOT found
		int GetUL(const char* name);
		// Returns -1 (just like OpenGL) if NOT found
		int GetAL(const char* name);

		// Loading uniform values
		void SetUniformFloat(const char* variableName, float value);
		void SetUniformInt(const char* variableName, int value);
		void SetUniformMatrix4f(const char* variableName, const glm::mat4& value);
		void SetUniformVec4(const char* variableName, const glm::vec4& value);
		void SetUniformVec3(const char* variableName, const glm::vec3& value);
		void SetUniformVec2(const char* variableName, const glm::vec2& value);

		void BindTexture(uint textureUnit, uint textureId);

	private:
		ShaderManager() {};

		bool m_WasThereACompileError(uint shaderID, std::string& errorText);
		bool m_WasThereALinkError(uint programID, std::string& errorText);
		size_t m_CreateProgramFromFile(std::string shaderProgram);
		size_t m_CreateComputeProgramFromFile(std::string shaderProgram);
		bool m_CompileShaderFromSource(uint shaderId,
										std::vector<std::string>& vecSource, 
										std::string& error);
		bool m_LoadSourceFromFile(std::string filename,
								  std::vector<std::string>& vecSource, 
								  std::string& errorText);

		// Look up the uniform location and save it.
		int m_LoadUniformLocation(const char* variableName);
		// Look up the attribute location and save it.
		bool m_LoadAttributeLocation(const char* variableName);

		std::string m_lastError;
		std::string m_basePath;

		std::map<std::string, std::shared_ptr<iResource>> m_mapShaders;
		std::vector<std::shared_ptr<iResource>> m_vecShaders;

		std::shared_ptr<iResource> m_defaultShader;

		// Current shader binded
		std::string m_currShader;
		int m_currShaderID;
	};
}