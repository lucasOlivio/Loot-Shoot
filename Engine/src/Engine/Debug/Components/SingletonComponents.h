#pragma once

#include "Engine/ECS/BaseComponent.h"
#include "Engine/Core/Resources/Meshes/Mesh.h"

namespace MyEngine
{
	struct DebugSquareComponent : public BaseComponent
	{
		std::shared_ptr<sMeshInfo> pMesh;
	};

	struct DebugSphereComponent : public BaseComponent
	{
		std::shared_ptr<sMeshInfo> pMesh;
	};
}