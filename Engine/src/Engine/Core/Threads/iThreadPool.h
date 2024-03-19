#pragma once

#include "Engine/Core/Threads/ThreadsProperties.h"

#include <Windows.h>

namespace MyEngine
{
	class Engine;

	class iThreadPool
	{
	public:
		iThreadPool() {};
		virtual ~iThreadPool() {};

		virtual void CreateWorkers() = 0;

		virtual void EnqueueTask(TaskHandler handler) = 0;

		virtual void CloseThreads() = 0;

		// Includes tasks waiting in queue to be executed
		virtual LONG RunningTasks() = 0;
	};
}
