#pragma once

#include "Engine/Core/StateProperties.h"

#include "Engine/ECS/Base.h"
#include "Engine/ThreadSafe.h"

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <map>
#include <vector>
#include <string>

namespace MyEngine
{
	struct TagComponent : public ThreadSafe
	{
		std::string name;
	};

	struct TransformComponent : public ThreadSafe
	{
		glm::vec3 position;
		glm::quat orientation;
		float scale;
	};

	struct ParentComponent : public ThreadSafe
	{
		Entity parentId;
	};
}
