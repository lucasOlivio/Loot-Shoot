#include "pch.h"

#include "ShaderProgram.h"
#include "ShaderManager.h"

#include "Engine/Core/Constants.h"

#include "Engine/Graphics/opengl.h"

namespace MyEngine
{
	void ShaderManager::SetBasePath(const std::string& basePath)
	{
		m_basePath = basePath;
	}

	size_t ShaderManager::LoadResource(const std::string& name)
	{
		size_t index = m_CreateProgramFromFile(name);

		return index;
	}

	void ShaderManager::DeleteResource(const std::string& name)
	{
		std::shared_ptr<iResource> pShader = GetResource(name);
		m_mapShaders.erase(name);
		m_vecShaders.erase(m_vecShaders.begin() + pShader->index);
	}

	void ShaderManager::DeleteResource(const size_t& index)
	{
		std::shared_ptr<iResource> pShader = GetResource(index);
		m_mapShaders.erase(pShader->name);
		m_vecShaders.erase(m_vecShaders.begin() + index);
	}

	void ShaderManager::ClearResources()
	{
		for (size_t i = 0; i < m_vecShaders.size();)
		{
			DeleteResource(i);
		}
	}

	std::shared_ptr<iResource> ShaderManager::GetResource(size_t index)
	{
		assert(index < m_vecShaders.size());

		return m_vecShaders.at(index);
	}

	std::shared_ptr<iResource> ShaderManager::GetResource(const std::string& name)
	{
		assert(m_mapShaders.find(name) != m_mapShaders.end());

		return m_mapShaders.at(name);
	}

	std::vector<std::shared_ptr<iResource>>& ShaderManager::GetResources()
	{
		return m_vecShaders;
	}

	void ShaderManager::ActivateResource(const std::string& name)
	{
		std::shared_ptr<ShaderProgram> pShader = std::static_pointer_cast<ShaderProgram>(GetResource(name));
		glUseProgram(pShader->ID);

		m_currShaderID = pShader->ID;
		m_currShader = name;
	}

	void ShaderManager::DeactivateResource()
	{
		glUseProgram(0);

		m_currShaderID = 0;
		m_currShader = "";
	}

	std::shared_ptr<iResource> ShaderManager::GetActiveResource()
	{
		return GetResource(m_currShader);
	}

	void ShaderManager::SetWireframe(bool isWireframe)
	{
		if (isWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	int ShaderManager::GetUL(const char* name)
	{
		std::shared_ptr<ShaderProgram> pShader = std::static_pointer_cast<ShaderProgram>(GetActiveResource());
		std::map< std::string /*name of uniform variable*/,
			int /* uniform location ID */ >::iterator
			itUniform = pShader->mapUniformToUL.find(name);
		if (itUniform == pShader->mapUniformToUL.end())
		{
			// Not in map yet, so load to map cache
			int uniLocation = m_LoadUniformLocation(name);
			return uniLocation;
		}

		return itUniform->second;		// second if the "int" value
	}

	int ShaderManager::GetAL(const char* name)
	{
		std::shared_ptr<ShaderProgram> pShader = std::static_pointer_cast<ShaderProgram>(GetActiveResource());
		std::map< std::string /*name of attribute variable*/,
			int /* attribute location ID */ >::iterator
			itAttribute = pShader->mapAttributeToAL.find(name);
		if (itAttribute == pShader->mapAttributeToAL.end())
		{
			// Not in map yet, so load to map cache
			bool ulFound = m_LoadAttributeLocation(name);
			if (!ulFound)
			{
				// OpenGL attribute not found value
				return -1;
			}

			itAttribute = pShader->mapAttributeToAL.find(name);
		}

		return itAttribute->second;		// second if the "int" value
	}

	void ShaderManager::SetUniformFloat(const char* variableName, float value)
	{
		GLint floatUL = GetUL(variableName);
		glUniform1f(floatUL, value);
	}

	void ShaderManager::SetUniformInt(const char* variableName, int value)
	{
		GLint intUL = GetUL(variableName);
		glUniform1i(intUL, value);
	}

	void ShaderManager::SetUniformMatrix4f(const char* variableName, const glm::mat4& value)
	{
		GLint matUL = GetUL(variableName);
		glUniformMatrix4fv(matUL, 1, GL_FALSE, glm::value_ptr(value));
	}

	void ShaderManager::SetUniformVec4(const char* variableName, const glm::vec4& value)
	{
		GLint vecUL = GetUL(variableName);
		glUniform4f(vecUL, value.x, value.y, value.z, value.w);
	}

	void ShaderManager::SetUniformVec3(const char* variableName, const glm::vec3& value)
	{
		GLint vecUL = GetUL(variableName);
		glUniform3f(vecUL, value.x, value.y, value.z);
	}

	void ShaderManager::SetUniformVec2(const char* variableName, const glm::vec2& value)
	{
		GLint vecUL = GetUL(variableName);
		glUniform2f(vecUL, value.x, value.y);
	}

	void ShaderManager::BindTexture(uint textureUnit, uint textureId)
	{
		glActiveTexture(textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	// Returns bool if didn't load
	bool ShaderManager::m_LoadSourceFromFile(std::string filename,
											 std::vector<std::string>& vecSource, 
											 std::string& errorText)
	{
		std::string fullFileName = m_basePath + filename;
		std::string info = "Opening shader file " + fullFileName;
		LOG_INFO(info);

		std::ifstream theFile(fullFileName.c_str());
		if (!theFile.is_open())
		{
			errorText = "Error: shader file " + fullFileName + " not found!\n";
			return false;
		}

		vecSource.clear();

		char* pLineTemp = new char[MAX_LINE_LENGTH];
		while (theFile.getline(pLineTemp, MAX_LINE_LENGTH))
		{
			std::string tempString(pLineTemp);
			vecSource.push_back(tempString);
		}

		theFile.close();
		return true;		// Return the string (from the sstream)
	}

	// Returns empty string if no error
	// returns false if no error
	bool ShaderManager::m_WasThereACompileError(uint shaderID, std::string& errorText)
	{
		errorText = "";	// No error

		GLint isCompiled = 0;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);

			char* pLogText = new char[maxLength];

			glGetShaderInfoLog(shaderID, maxLength, &maxLength, pLogText);
			// Copy char array to string
			errorText.append(pLogText);

			m_lastError.append("\n");
			m_lastError.append(errorText);


			delete[] pLogText;

			return true;	// There WAS an error
		}
		return false; // There WASN'T an error
	}

	bool ShaderManager::m_WasThereALinkError(uint programID, std::string& errorText)
	{
		errorText = "";	// No error

		GLint wasError = 0;
		glGetProgramiv(programID, GL_LINK_STATUS, &wasError);
		if (wasError == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

			char* pLogText = new char[maxLength];
			// Fill with zeros, maybe...?
			glGetProgramInfoLog(programID, maxLength, &maxLength, pLogText);
			// Copy char array to string
			errorText.append(pLogText);

			// Extra code that Michael forgot wasn't there... 
			m_lastError.append("\n");
			m_lastError.append(errorText);


			delete[] pLogText;

			// There WAS an error
			return true;
		}

		// There WASN'T an error
		return false;
	}

	size_t ShaderManager::m_CreateProgramFromFile(std::string shaderProgram)
	{
		std::string errorText = "";

		Shader vertexShader;
		vertexShader.fileName = "vertex" + shaderProgram + ".glsl";

		Shader fragmentShader;
		fragmentShader.fileName = "fragment" + shaderProgram + ".glsl";

		// Shader loading happening before vertex buffer array
		vertexShader.ID = glCreateShader(GL_VERTEX_SHADER);
		vertexShader.shaderType = eShaderType::VERTEX_SHADER;

		// Load some text from a file...
		bool textLoaded = m_LoadSourceFromFile(vertexShader.fileName, vertexShader.vecSource, errorText);
		assert(textLoaded);

		errorText = "";
		bool vertexLoaded = m_CompileShaderFromSource(vertexShader.ID, vertexShader.vecSource, errorText);
		assert(vertexLoaded);

		fragmentShader.ID = glCreateShader(GL_FRAGMENT_SHADER);
		fragmentShader.shaderType = eShaderType::FRAGMENT_SHADER;
		bool textFragLoaded = m_LoadSourceFromFile(fragmentShader.fileName, fragmentShader.vecSource, errorText);
		assert(textFragLoaded);

		bool fragLoaded = m_CompileShaderFromSource(fragmentShader.ID, fragmentShader.vecSource, errorText);
		assert(fragLoaded);

		std::shared_ptr<ShaderProgram> curProgram(new ShaderProgram());
		curProgram->ID = glCreateProgram();

		glAttachShader(curProgram->ID, vertexShader.ID);
		glAttachShader(curProgram->ID, fragmentShader.ID);
		glLinkProgram(curProgram->ID);

		// Was there a link error? 
		errorText = "";
		bool isError = m_WasThereALinkError(curProgram->ID, errorText);
		assert(!isError);

		// At this point, shaders are compiled and linked into a program
		curProgram->name = shaderProgram;

		// Add the shader to cache
		m_mapShaders[curProgram->name] = curProgram;
		curProgram->index = m_vecShaders.size();
		m_vecShaders.push_back(curProgram);

		return curProgram->index;
	}

	bool ShaderManager::m_CompileShaderFromSource(uint shaderId,
												  std::vector<std::string>& vecSource, 
												  std::string& error)
	{
		error = "";

		uint numberOfLines = static_cast<uint>(vecSource.size());

		// This is an array of pointers to strings. aka the lines of source
		char** arraySource = new char* [numberOfLines];
		// Clear array to all zeros (i.e. '\0' or null terminator)
		memset(arraySource, 0, numberOfLines);

		LOG_INFO("Loading shader text...\n");
		for (uint indexLine = 0; indexLine != numberOfLines; indexLine++)
		{
			uint numCharacters = (uint)vecSource[indexLine].length();
			// Create an array of chars for each line
			arraySource[indexLine] = new char[numCharacters + 2];		// For the '\n' and '\0' at end
			memset(arraySource[indexLine], 0, numCharacters + 2);;

			// The "secure" version to copy arrays, where you specify the number of chars to copy
			strcpy_s(arraySource[indexLine],
				strlen(vecSource[indexLine].c_str()) + 1,	// Number of char in copy + 0 terminator
				vecSource[indexLine].c_str());
			arraySource[indexLine][numCharacters + 0] = '\n';
			arraySource[indexLine][numCharacters + 1] = '\0';
		}

		LOG_INFO("Compiling shader..\n");
		glShaderSource(shaderId, numberOfLines, arraySource, NULL);
		glCompileShader(shaderId);

		// Get rid of the temp source "c" style array
		for (uint indexLine = 0; indexLine != numberOfLines; indexLine++)
		{	// Delete this line
			delete[] arraySource[indexLine];
		}
		// And delete the original char** array
		delete[] arraySource;

		// Did it work? 
		std::string errorText = "";
		if (m_WasThereACompileError(shaderId, errorText))
		{
			std::stringstream ssError;
			ssError << " compile error: ";
			ssError << errorText;
			error = ssError.str();
			return false;
		}

		return true;
	}

	int ShaderManager::m_LoadUniformLocation(const char* variableName)
	{
		std::shared_ptr<ShaderProgram> pShader = std::static_pointer_cast<ShaderProgram>(GetActiveResource());
		int uniLocation = glGetUniformLocation(m_currShaderID, variableName);
		// Did it find it (not -1)
		if (uniLocation > -1)
		{
			// Save it
			pShader->mapUniformToUL[variableName] = uniLocation;
		}

		return uniLocation;
	}

	bool ShaderManager::m_LoadAttributeLocation(const char* variableName)
	{
		std::shared_ptr<ShaderProgram> pShader = std::static_pointer_cast<ShaderProgram>(GetActiveResource());
		GLint attrLocation = glGetAttribLocation(m_currShaderID, variableName);
		// Did it find it (not -1)
		if (attrLocation == -1)
		{	// Nope.
			return false;
		}
		// Save it
		pShader->mapAttributeToAL[variableName] = attrLocation;

		return true;
	}
}
