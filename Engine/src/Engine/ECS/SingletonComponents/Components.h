#pragma once

#include "Engine/Core/Shapes.hpp"

#include "Engine/ECS/Base.h"
#include "Engine/ECS/Scene/Scene.h"

#include "Engine/Graphics/opengl.h"
#include "Engine/Graphics/Textures/TextureProperties.h"
#include "Engine/Graphics/GraphicsProperties.h"

#include "Engine/Physics/BroadPhase/GridAABB.h"
#include "Engine/Physics/PhysicsProperties.h"

#include "Engine/Utils/TransformUtils.h"

#include <glm/gtc/quaternion.hpp>
#include <glm/mat4x4.hpp>
#include <map>

namespace MyEngine
{
	// Singleton components

	

	// Debug objects

	struct DebugSquareComponent
	{
		sMesh* pMesh;

		std::vector<uint> FBOIDs = { 0 };
	};

	struct DebugSphereComponent
	{
		sMesh* pMesh;

		std::vector<uint> FBOIDs = { 0 };
	};
}