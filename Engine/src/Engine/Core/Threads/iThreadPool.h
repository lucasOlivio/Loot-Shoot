#pragma once

#include <functional>
#include <Windows.h>

namespace MyEngine
{
	class Engine;

	class iThreadPool
	{
	public:
		using Task = std::function<void()>;

		iThreadPool() {};
		virtual ~iThreadPool() {};

		virtual void CreateWorkers() = 0;

		virtual void EnqueueTask(Task task) = 0;

		virtual void CloseThreads() = 0;

		// Includes tasks waiting in queue to be executed
		virtual LONG RunningTasks() = 0;
	};
}
