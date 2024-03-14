#pragma once

#include "Engine/Core/Threads/iThreadPool.h"

#include "Engine/Utils/Log.h"

namespace MyEngine
{
	class NullThreadPool : public iThreadPool
	{
	public:
		NullThreadPool() {};
		virtual ~NullThreadPool() {};

		virtual void CreateWorkers()
		{
			LOG_ERROR("Thread pool not created!");
		}

		virtual void EnqueueTask(Task task)
		{
			LOG_ERROR("Thread pool not created!");
		}

		virtual void CloseThreads()
		{
			LOG_ERROR("Thread pool not created!");
		}

		virtual LONG RunningTasks()
		{
			LOG_ERROR("Thread pool not created!");
			return 0;
		}
	};
}
