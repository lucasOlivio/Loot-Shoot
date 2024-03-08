#pragma once

#include "Engine/Core/Constants.h"

#include "Engine/ECS/iComponent.h"

#include "Engine/Physics/PhysicsProperties.h"
#include "Engine/Physics/BroadPhase/GridAABB.h"

#include <map>
#include <vector>
#include <set>

namespace MyEngine
{
	struct GridBroadphaseComponent : public iComponent
	{
		// Mapping of AABBs to their indices in the grid
		std::map< uint /*index*/, GridAABB* > mapAABBs;

		// Length per grid box in the grid
		glm::vec3 lengthPerBox;
	};

	// Generic grouping for any narrow phase testing 
	struct NarrowPhaseTestsComponent : public iComponent
	{
		// Groups of shapes and entities to test,
		// the triangles to be tested against the entities will be in the same index
		std::vector<std::vector<Entity>> passiveEntitiesToTest;
		std::vector<std::vector<Entity>> staticEntitiesToTest;
		std::vector<std::vector<Entity>> activeEntitiesToTest;
	};

	// All collisions that happened in the last <FRAME_RATE> frames
	struct FrameCollisionComponent : public iComponent
	{
		std::set<sCollisionData> collisions[FRAME_RATE];
		std::set<sCollisionParticleData> particleCollisions[FRAME_RATE];
		size_t currSize;
		size_t currParticlesSize;
	};
}
