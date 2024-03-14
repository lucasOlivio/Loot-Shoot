#pragma once

#include "Engine/Core/Threads/iThreadPool.h"

#include <queue>

namespace MyEngine
{
	class ThreadPool : public iThreadPool
	{
	public:
		ThreadPool();
		virtual ~ThreadPool();

		virtual void CreateWorkers();

		virtual void EnqueueTask(Task task);

		virtual void CloseThreads();

		virtual LONG RunningTasks();

	private:
		static constexpr int MAX_THREADS = 64;

		bool m_isRunning;

		// 0 - Event for workers task added to queue 
		// 1 - Event for closing threads
		HANDLE m_events[2];

		// Task queue and synchronization primitives
		std::queue<Task> m_taskQueue;
		CRITICAL_SECTION m_CSTasks;

		LONG m_runningTasks;
		HANDLE m_vecThreads[MAX_THREADS];
		static DWORD WINAPI m_WorkerThread(LPVOID lpParam);
	};
}
