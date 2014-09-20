#include "Thread.h"
#include <process.h>

Thread::Thread() : m_hThread(nullptr)
{
	StartThread();
}
	
Thread::~Thread()
{
	KillThread();
}

unsigned int __stdcall Thread::DefaultThreadProc(void* pArgs)
{
	u32 nProcRetVal = (u32)-1;

	if(pArgs)
	{
		ThreadArgs& threadArgs = *(ThreadArgs*)pArgs;
		WaitObject& jobWaitObject = threadArgs.m_JobWaitObject;
		int jobRetVal;

		while(true)
		{
			jobWaitObject.Wait();

			if(threadArgs.m_pThreadFunc)
			{
				jobRetVal = (threadArgs.m_pThreadFunc)(threadArgs.m_pArgs);

				if(threadArgs.m_pFinishedCallback)
					(threadArgs.m_pFinishedCallback)(jobRetVal, threadArgs.m_pArgs);

				threadArgs.Reset();
			}
			else
			{
				break;
			}
		}

		nProcRetVal = 0;
	}

#if __ASSERT
	if(nProcRetVal == -1)
	{
		Assert(false, "Failed To Properly Start Thread!");
	}
#endif // __ASSERT

	return nProcRetVal;
}

inline void Thread::CloseThread()
{
	if(m_hThread)
	{
		AssignTask(nullptr, nullptr);
		m_ThreadWaitObject.Wait();

		BOOL bHandleClosed = CloseHandle(m_hThread);

		if(!bHandleClosed)
		{
			DisplayDebugString("Failed To Close Handle (%p), Error Number: %d", GetLastError());
		}

		m_hThread = nullptr;
	}
}

inline void Thread::ExitThread()
{
	_endthreadex(THREAD_KILL_RET_VAL);
}

void Thread::AssignTask(ThreadProc pProc, void* pArgs, ThreadFinishCallback pFinishedCallback/* = nullptr*/, bool bAutoSignalWork /*= true*/)
{
	if(m_hThread && !m_ThreadArgs.m_pThreadFunc)
	{
		m_ThreadArgs.m_pArgs = pArgs;
		m_ThreadArgs.m_pThreadFunc = pProc;
		m_ThreadArgs.m_pFinishedCallback = pFinishedCallback;

		if(bAutoSignalWork)
			m_ThreadArgs.m_JobWaitObject.Signal();
	}
}

void Thread::SignalWork()
{
	if(m_hThread && !m_ThreadArgs.m_pThreadFunc)
	{
		m_ThreadArgs.m_JobWaitObject.Signal();
	}
}

bool Thread::StartThread()
{
	bool bThreadStarted = false;

	if(Verify(!m_hThread, "Thread is Already Active, Start Thread will notdo anything."))
	{
		//if(Verify(m_pThreadFunc, "No Function Set for the starting thread"))
		{
			m_hThread = (void*)_beginthreadex(NULL, 0, Thread::DefaultThreadProc, &m_ThreadArgs, 0, NULL);
			Sleep(1000);
			m_ThreadWaitObject.SetWaitHandle(m_hThread);
		}

		return true;
	}

	return bThreadStarted;
}

void Thread::KillThread()
{
	if(m_hThread)
	{
		//ExitThread();
		CloseThread();
	}
}