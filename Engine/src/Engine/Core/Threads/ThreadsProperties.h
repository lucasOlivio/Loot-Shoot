#pragma once

#include "Engine/Core/types.h"

#include <functional>

namespace MyEngine
{
	using TaskHandler = std::function<void()>;

	struct Task
	{
		uint ID = 0;
		TaskHandler handler;
	};
}
