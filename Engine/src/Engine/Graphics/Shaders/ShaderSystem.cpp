#include "pch.h"

#include "ShaderSystem.h"

#include "Engine/Core/Resources/ResourceManagerFactory.h"

namespace MyEngine
{
	void ShaderSystem::Init()
	{
		std::shared_ptr<iResourceManager> pShader = ResourceManagerFactory::GetOrCreate(eResourceTypes::SHADER);

		// Setup shaders
		std::string shaderName = "Shader01"; // TODO: This should come from config?
		size_t index = pShader->LoadResource(shaderName);
		pShader->ActivateResource(shaderName);
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
