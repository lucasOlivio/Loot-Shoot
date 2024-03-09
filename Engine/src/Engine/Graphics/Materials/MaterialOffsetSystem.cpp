#include "pch.h"

#include "MaterialOffsetSystem.h"

#include "Engine/Core/Resources/iResourceManager.h"
#include "Engine/Core/Resources/ResourceManagerFactory.h"
#include "Engine/Core/Resources/Materials/Material.h"

#include "Engine/Graphics/GraphicsProperties.h"

#include "Engine/ECS/Scene/SceneView.hpp"

namespace MyEngine
{
	void MaterialOffsetSystem::Init()
	{
	}

	void MaterialOffsetSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void MaterialOffsetSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
		std::shared_ptr<iResourceManager> pMaterialManager = ResourceManagerFactory::CreateResManager(eResourceTypes::MATERIAL);
		std::vector<std::shared_ptr<iResource>>& materials = pMaterialManager->GetResources();

		// Increase offset for all materials
		for (std::shared_ptr<iResource> pResource : materials)
		{
			std::shared_ptr<sMaterialInfo> pMaterial = std::static_pointer_cast<sMaterialInfo>(pResource);

			// Position offset
			pMaterial->currOffset += pMaterial->offsetMove * deltaTime;

			float resetValue = 1.0;
			if (pMaterial->useCubeTexture)
			{
				resetValue = 2.0;
			}
			// Clamp offset between 0 and 1
			if (pMaterial->currOffset.x > 1)
			{
				pMaterial->currOffset.x = 0;
			}
			if (pMaterial->currOffset.y > 1)
			{
				pMaterial->currOffset.y = 0;
			}

			// Height map offset
			if (pMaterial->offsetHeightMap == glm::vec3(0))
			{
				continue;
			}

			pMaterial->currOffsetHeightMap += pMaterial->offsetHeightMap * deltaTime;

			// Clamp offset between 0 and 1
			if (pMaterial->currOffsetHeightMap.x > 1)
			{
				pMaterial->currOffsetHeightMap.x = 0;
			}
			if (pMaterial->currOffsetHeightMap.y > 1)
			{
				pMaterial->currOffsetHeightMap.y = 0;
			}
		}
	}

	void MaterialOffsetSystem::Render(std::shared_ptr<Scene> pScene)
	{
	}

	void MaterialOffsetSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void MaterialOffsetSystem::Shutdown()
	{
	}
}