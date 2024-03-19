#include "pch.h"

#include "ThreadPool.h"

#include "Engine/Core/EngineLocator.h"

#include "Engine/ECS/Scene/Scene.h"

#include "Engine/Utils/WindowsUtils.h"

namespace MyEngine
{
	ThreadPool::ThreadPool() : m_isRunning(false), 
							m_runningTasks(0), 
							m_CSTasks(CRITICAL_SECTION()),
							m_nextId(0)
	{
	}

	void ThreadPool::CreateWorkers()
	{
		InitializeCriticalSection(&m_CSTasks);

		m_isRunning = true;

		// New task event
		m_events[0] = CreateEvent(NULL,        // default security attributes
								FALSE,         // auto-reset event
								FALSE,         // initial state is nonsignaled
								NULL);

		if (m_events[0] == NULL)
		{
			LOG_ERROR("ThreadPool::CreateWorkers NewTask CreateEvent failed (" + std::to_string(GetLastError()) + ") :" + WindowsUtils::GetLastErrorAsString());
			return;
		}


		m_events[1] = CreateEvent(NULL,        // default security attributes
								TRUE,          // manual-reset event
								FALSE,         // initial state is nonsignaled
								NULL);

		if (m_events[1] == NULL)
		{
			LOG_ERROR("ThreadPool::CreateWorkers Close CreateEvent failed (" + std::to_string(GetLastError()) + ") :" + WindowsUtils::GetLastErrorAsString());
			return;
		}

		for (int i = 0; i < MAX_THREADS; ++i) {
			m_vecThreads[i] = CreateThread(NULL, 0, ThreadPool::m_WorkerThread, this, 0, NULL);

			if (m_vecThreads[i] == NULL)
			{
				LOG_ERROR("ThreadPool::CreateWorkers Failed to create thread handle!");
				return;
			}
		}
	}

	void ThreadPool::EnqueueTask(TaskHandler handler)
	{
		InterlockedIncrement(&m_runningTasks);

		Task task = Task();
		task.ID = m_nextId;
		task.handler = handler;
		EnterCriticalSection(&m_CSTasks);
		m_taskQueue.push(task);
		LeaveCriticalSection(&m_CSTasks);

		// Send new task event
		if (!SetEvent(m_events[0]))
		{
			LOG_ERROR("ThreadPool::EnqueueTask SetEvent failed (" + std::to_string(GetLastError()) + ") :" + WindowsUtils::GetLastErrorAsString());
			return;
		}

		InterlockedIncrement(&m_nextId);
	}

	void ThreadPool::CloseThreads()
	{
		m_isRunning = false;

		// Send close event
		if (!SetEvent(m_events[1]))
		{
			LOG_ERROR("ThreadPool::EnqueueTask SetEvent failed (" + std::to_string(GetLastError()) + ") :" + WindowsUtils::GetLastErrorAsString());
			return;
		}

		DWORD result = WaitForMultipleObjects(MAX_THREADS,   // number of handles in array
											  m_vecThreads,  // array of thread handles
											  TRUE,          // wait until all are signaled
											  INFINITE);

		switch (result)
		{
			// All thread objects were signaled
		case WAIT_OBJECT_0:
			break;

			// An error occurred
		default:
			LOG_ERROR("ThreadPool::CloseThreads WaitForMultipleObjects failed (" + std::to_string(GetLastError()) + ") :" + WindowsUtils::GetLastErrorAsString());
			return;
		}

		DeleteCriticalSection(&m_CSTasks);
		CloseHandle(m_events[0]);
		CloseHandle(m_events[1]);
	}

	LONG ThreadPool::RunningTasks()
	{
		return m_runningTasks;
	}

	DWORD WINAPI ThreadPool::m_WorkerThread(LPVOID lpParam)
	{
		ThreadPool* threadPool = static_cast<ThreadPool*>(lpParam);

		while (threadPool->m_isRunning)
		{
			DWORD result = WaitForMultipleObjects(2, threadPool->m_events,
													FALSE,       // wait for any object
													INFINITE);

			switch (result)
			{
			// m_events[0] was signaled
			case WAIT_OBJECT_0:
			{
				// Execute while there are tasks available
				Task task;
				while (threadPool->m_GetNextTask(task))
				{
					task.handler();
					InterlockedDecrement(&threadPool->m_runningTasks);
				}

				break;
			}
			// m_events[1] was signaled
			case WAIT_OBJECT_0 + 1:
				InterlockedDecrement(&threadPool->m_runningTasks);
				break;
			default:
				LOG_ERROR("ThreadPool::m_WorkerThread Wait error (" + std::to_string(GetLastError()) + ") :" + WindowsUtils::GetLastErrorAsString());
				return 0;
			}
		}

		return 0;
	}

	bool ThreadPool::m_GetNextTask(Task& task)
	{
		EnterCriticalSection(&m_CSTasks);
		if (m_taskQueue.empty())
		{
			LeaveCriticalSection(&m_CSTasks);
			return false;
		}

		task = m_taskQueue.front();
		m_taskQueue.pop();
		LeaveCriticalSection(&m_CSTasks);

		return true;
	}
}
