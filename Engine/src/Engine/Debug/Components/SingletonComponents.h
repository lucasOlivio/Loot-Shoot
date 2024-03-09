#pragma once

#include "Engine/Core/Resources/Meshes/Mesh.h"

namespace MyEngine
{
	struct DebugSquareComponent
	{
		std::shared_ptr<sMeshInfo> pMesh;
	};

	struct DebugSphereComponent
	{
		std::shared_ptr<sMeshInfo> pMesh;
	};
}