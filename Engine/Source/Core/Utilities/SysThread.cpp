#include "SysThread.h"
#include <process.h>

SysThread::SysThread()
{
}

SysThread::~SysThread()
{
	EndThread();
}

bool SysThread::StartThread(SysThreadProc pThreadProc, void* pArgs, eThreadPriority eThreadPrio /*= THREAD_PRIO_NORMAL*/, char* szThreadName /*= nullptr*/)
{
	bool bThreadStarted = false;

	if(Verify(!m_pHandle, "Thread is Already Active, Start Thread will not do anything."))
	{
		if(Verify(!pThreadProc.IsNull(), "No Function Set for the starting thread"))
		{
			m_ThreadArgs.m_pThreadFunc = pThreadProc;
			m_ThreadArgs.m_pArgs = pArgs;
			m_ThreadArgs.m_ePriority = eThreadPrio;
			m_ThreadArgs.m_szThreadName = szThreadName;

			m_pHandle = (void*)_beginthreadex(NULL, 0, DefaultThreadProc, &m_ThreadArgs, 0, NULL);

			if(m_pHandle)
			{
				bThreadStarted = true;
			}
			else
			{
				DisplayDebugString("Failed To Create New Thread!");
			}
		}
	}

	return bThreadStarted;
}

void SysThread::CloseThread()
{
	if(m_pHandle)
	{
		ASSERT_ONLY(BOOL bHandleClosed = ) CloseHandle(m_pHandle);

#if __ASSERT
		if(!bHandleClosed)
		{
			DisplayDebugString("Failed To Close Handle (%p), Error Number: %d", m_pHandle, GetLastError());
		}
#endif // __ASSERT

		m_pHandle = nullptr;
	}
}

unsigned int __stdcall SysThread::DefaultThreadProc(void* pArgs)
{
	u32 nProcRetVal = (u32)-1;

	if(pArgs)
	{
		ThreadArgs& threadArgs = *(ThreadArgs*)pArgs;

		SysSetThreadName(threadArgs.m_szThreadName);

		SysSetThreadPriority(threadArgs.m_ePriority);

		(threadArgs.m_pThreadFunc)(threadArgs.m_pArgs);

		nProcRetVal = 0;
	}

#if __ASSERT
	if(nProcRetVal == -1)
	{
		Assert(false, "Failed To Properly Start Thread! pArgs must be NULL for some reason!");
	}
#endif // __ASSERT

	return nProcRetVal;
}

void SysThread::EndThread()
{
	if(m_pHandle)
	{
		Wait();
		CloseThread();
	}
}

void SysThread::KillThread()
{
	if(m_pHandle)
	{
		TerminateThread(m_pHandle, THREAD_KILL_RET_VAL);
	}
}

// hThread - Pass nullptr to set Calling Thread's Name
void SysSetThreadName(const char* szName, void* hThread /*= nullptr*/)
{
	// How to Set Thread Name According to MSDN

	const DWORD MS_VC_EXCEPTION = 0x406D1388;

	struct THREADNAME_INFO
	{
		const DWORD dwType = 0x1000; // Must be 0x1000.
		LPCSTR szName; // Pointer to name (in user addr space).
		DWORD dwThreadID; // Thread ID (-1=caller thread).
		DWORD dwFlags; // Reserved for future use, must be zero.
	};

	THREADNAME_INFO info;
	info.szName = szName ? szName : "Worker Thread";
	info.dwThreadID = hThread ? GetCurrentThreadId() : (DWORD)-1;
	info.dwFlags = 0;

	__try
	{
		RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
	}
	__except(EXCEPTION_EXECUTE_HANDLER) {}
}

void SysSetThreadPriority(eThreadPriority eThreadPrio, void* hThread /*= nullptr*/)
{
	if(Verify((eThreadPrio == THREAD_PRIO_IDLE || eThreadPrio == THREAD_PRIO_CRITICAL) ||
		(eThreadPrio >= THREAD_PRIO_LOW && eThreadPrio <= THREAD_PRIO_HIGH), "SysSetThreadPriority() - Invalid Thread Priority"))
	{
		SetThreadPriority(hThread ? hThread : GetCurrentThread(), eThreadPrio);
	}
}