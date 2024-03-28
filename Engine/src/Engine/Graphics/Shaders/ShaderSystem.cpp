#include "pch.h"

#include "ShaderSystem.h"

#include "Engine/Core/Resources/ResourceManagerFactory.h"

namespace MyEngine
{
	void ShaderSystem::Init()
	{
		std::shared_ptr<iResourceManager> pShader = ResourceManagerFactory::GetOrCreate(eResourceTypes::SHADER);

		// Setup shaders
		pShader->LoadResource(PARTICLES_SHADER);

		pShader->LoadResource(DEFAULT_SHADER);
		pShader->ActivateResource(DEFAULT_SHADER);
	}

	void ShaderSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void ShaderSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
	}

	void ShaderSystem::Render(std::shared_ptr<Scene> pScene, float deltaTime)
	{
	}

	void ShaderSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void ShaderSystem::Shutdown()
	{
	}
}
