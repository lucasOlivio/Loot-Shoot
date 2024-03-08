#pragma once

#include "Engine/Core/StateProperties.h"

#include "Engine/ECS/Base.h"
#include "Engine/ECS/iComponent.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <vector>
#include <string>

namespace MyEngine
{
	struct TransformComponent : public iComponent
	{
		glm::vec3 position;
		glm::quat orientation;
		float scale;

		// Values calculated in relation to parent
		glm::vec3 worldPosition;
		glm::quat worldOrientation;
		float worldScale;

		// Value calculated distance in relation to camera position
		float distToCamera;
	};

	struct ParentComponent : public iComponent
	{
		Entity parentId;
	};
}
