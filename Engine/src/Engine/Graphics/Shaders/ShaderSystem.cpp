#include "pch.h"

#include "ShaderSystem.h"
#include "Engine/Core/Components/CoreLocator.h"
#include "Engine/Graphics/Shaders/ShaderManagerLocator.h"
#include "Engine/Graphics/Shaders/ShaderManager.h"

namespace MyEngine
{
	void ShaderSystem::Init()
	{
		std::shared_ptr<ConfigPathComponent> pConfigPath = CoreLocator::GetConfigPath();
		std::shared_ptr<iShaderManager> pShaderManager = ShaderManagerLocator::Get();

		// Setup shaders
		pShaderManager->SetBasePath(pConfigPath->pathShaders);
		std::string shaderName = "Shader01"; // TODO: This should come from config?
		pShaderManager->AddShaderProgram(shaderName);
		pShaderManager->UseShaderProgram(shaderName);
	}

	void ShaderSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void ShaderSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
	}

	void ShaderSystem::Render(std::shared_ptr<Scene> pScene)
	{
	}

	void ShaderSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void ShaderSystem::Shutdown()
	{
	}
}
