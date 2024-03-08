#include "pch.h"

#include "DrawGridSystem.h"

#include "Engine/Graphics/Renderer/RendererManagerLocator.h"

#include "Engine/ECS/SingletonComponents/PhysicsLocator.h"
#include "Engine/ECS/SingletonComponents/DebugLocator.h"

#include "Engine/Utils/TransformUtils.h"

namespace MyEngine
{
	void DrawGridSystem::Init()
	{
		// Just a check if the debug square mesh loaded
		std::shared_ptr<DebugSquareComponent> pSquare = DebugLocator::GetSquare();

		sMesh* pMesh = pSquare->pMesh;
		if (!pMesh)
		{
			LOG_WARNING("Debug Square not loaded!");
			return;
		}
	}

	void DrawGridSystem::Start(std::shared_ptr<Scene> pScene)
	{
	}

	void DrawGridSystem::Update(std::shared_ptr<Scene> pScene, float deltaTime)
	{
	}

	void DrawGridSystem::Render(std::shared_ptr<Scene> pScene)
	{
		std::shared_ptr<iRendererManager> pRendererManager = RendererManagerLocator::Get();

		std::shared_ptr<GridBroadphaseComponent> pGrid = PhysicsLocator::GetGridBroadphase();
		std::shared_ptr<DebugSquareComponent> pSquare = DebugLocator::GetSquare();

		sMesh* pMesh = pSquare->pMesh;

		if (!pMesh)
		{
			return;
		}

		// Draw a red wireframe square for every AABB in the grid
		for (std::pair<uint, GridAABB*> pairAABB : pGrid->mapAABBs)
		{
			GridAABB* pAABB = pairAABB.second;

			glm::mat4 matTransf = glm::mat4(1.0f);
			TransformUtils::GetTransform(pAABB->minXYZ, pGrid->lengthPerBox.x, matTransf);

			sRenderModelInfo renderInfo = sRenderModelInfo();
			renderInfo.matModel = matTransf;
			renderInfo.VAO_ID = pMesh->VAO_ID;
			renderInfo.numberOfIndices = pMesh->numberOfIndices;
			renderInfo.isWireFrame = true;
			renderInfo.doNotLight = true;
			renderInfo.useDebugColor = true;
			renderInfo.debugColor = RED;

			pRendererManager->AddToRender(renderInfo);
		}
	}

	void DrawGridSystem::End(std::shared_ptr<Scene> pScene)
	{
	}

	void DrawGridSystem::Shutdown()
	{
	}
}
