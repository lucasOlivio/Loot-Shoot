#pragma once

#include "Engine/Utils/Log.h"
#include "Engine/Core/Threads/iThreadPool.h"

namespace MyEngine
{
	class NullThreadPool : public iThreadPool
	{
	public:
		NullThreadPool() {};
		virtual ~NullThreadPool() {};

		virtual void CreateWorkers()
		{
			LOG_ERROR("Thread pool not initialized!");
		}

		virtual void EnqueueTask(TaskHandler handler)
		{
			LOG_ERROR("Thread pool not initialized!");
		}

		virtual void CloseThreads()
		{
			LOG_ERROR("Thread pool not initialized!");
		}

		// Includes tasks waiting in queue to be executed
		virtual LONG RunningTasks()
		{
			LOG_ERROR("Thread pool not initialized!");
			return 0;
		}
	};
}
