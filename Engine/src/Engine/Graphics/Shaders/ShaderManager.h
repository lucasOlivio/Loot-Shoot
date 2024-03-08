#pragma once

#include "iShaderManager.h"
#include <glm/mat4x4.hpp>
#include <vector>
#include <map>

namespace MyEngine
{
	class ShaderManager : public iShaderManager
	{
	public:
		ShaderManager() {};
		ShaderManager(std::string basePath);
		virtual ~ShaderManager();

		// Where we initialize all things that could go wrong
		virtual bool AddShaderProgram(std::string shaderProgram);

		virtual bool CreateProgramFromFile(std::string shaderProgram,
									std::string vertexName,
									std::string fragName);
		virtual void SetBasePath(std::string basepath);

		// Clears last error
		virtual std::string GetLastError(void);

		virtual uint GetIDFromShaderProgramName(std::string& shaderProgram);

		// Used to load the uniforms. Returns NULL if not found.
		virtual std::shared_ptr<iShaderProgram> GetShaderProgramFromName(std::string& shaderProgram);
		virtual std::shared_ptr<iShaderProgram> GetShaderProgramFromID(uint shaderProgramID);

		// Bind opengl to this shader program
		virtual bool UseShaderProgram(uint ID);
		virtual bool UseShaderProgram(std::string& shaderProgram);

		static std::shared_ptr<iShaderProgram> GetActiveShader();

	private:
		// Returns an empty string if it didn't work
		bool m_LoadSourceFromFile(std::string filename, 
								  std::vector<std::string>& vecSource, 
								  std::string& errorText);

		bool m_CompileShaderFromSource(uint shaderId,
									   std::vector<std::string>& vecSource, 
									   std::string& error);
		// returns false if no error
		bool m_WasThereACompileError(uint shaderID, std::string& errorText);
		bool m_WasThereALinkError(uint progID, std::string& errorText);

		std::string m_basepath;

		std::string m_lastError;

		static std::shared_ptr<iShaderProgram> m_pActiveShader;

		std::map< uint /*ID*/, std::shared_ptr<iShaderProgram> > m_IDToShaderProgram;
		std::map< std::string /*name*/, uint /*ID*/ > m_shaderProgramNameToID;
	};
}