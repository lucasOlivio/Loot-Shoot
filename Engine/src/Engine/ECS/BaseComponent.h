#pragma once

#include <Windows.h>

namespace MyEngine
{
    // Thread safe component, allow multiple readers but only one writer at a time
    struct BaseComponent
    {
    public:
        BaseComponent() 
        {
            InitializeCriticalSection(&m_csWriter);
            m_readers = 0;
        }

        ~BaseComponent() 
        {
            DeleteCriticalSection(&m_csWriter);
        }

        void LockRead() 
        {
            InterlockedIncrement(&m_readers);
            if (m_readers == 1) 
            {
                EnterCriticalSection(&m_csWriter);
            }
        }

        void UnlockRead() 
        {
            InterlockedDecrement(&m_readers);
            if (m_readers == 0) 
            {
                LeaveCriticalSection(&m_csWriter);
            }
        }

        void LockWrite() 
        {
            EnterCriticalSection(&m_csWriter);
        }

        void UnlockWrite() 
        {
            LeaveCriticalSection(&m_csWriter);
        }

    private:
        CRITICAL_SECTION m_csWriter; // Critical section for controlling access to writer
        LONG m_readers;              // Number of active readers
    };
}
