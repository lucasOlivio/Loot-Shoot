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

	struct GridBroadphaseComponent
	{
		// Mapping of AABBs to their indices in the grid
		std::map< uint /*index*/, GridAABB* > mapAABBs;

		// Length per grid box in the grid
		glm::vec3 lengthPerBox;
	};

	// Generic grouping for any narrow phase testing 
	struct NarrowPhaseTestsComponent
	{
		// Groups of shapes and entities to test,
		// the triangles to be tested against the entities will be in the same index
		std::vector<std::vector<Entity>> passiveEntitiesToTest;
		std::vector<std::vector<Entity>> staticEntitiesToTest;
		std::vector<std::vector<Entity>> activeEntitiesToTest;
	};

	// All collisions that happened in the last <FRAME_RATE> frames
	struct FrameCollisionComponent
	{
		std::set<sCollisionData> collisions[FRAME_RATE];
		std::set<sCollisionParticleData> particleCollisions[FRAME_RATE];
		size_t currSize;
		size_t currParticlesSize;
	};

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