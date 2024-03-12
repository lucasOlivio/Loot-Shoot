#pragma once

#include "Engine/ThreadSafe.h"
#include "Engine/Core/Resources/Meshes/Mesh.h"

namespace MyEngine
{
	struct DebugSquareComponent : public ThreadSafe
	{
		std::shared_ptr<sMeshInfo> pMesh;
	};

	struct DebugSphereComponent : public ThreadSafe
	{
		std::shared_ptr<sMeshInfo> pMesh;
	};
}