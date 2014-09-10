#include "Thread.h"
#include "UtilitiesInclude.h"
#include <process.h>
#include <WinBase.h>

Thread::Thread() : m_hThread(nullptr), m_pArgs(nullptr), m_pThreadFunc(nullptr), m_pFinishedCallback(nullptr)
{

}
	
Thread::~Thread()
{
	KillThread();
}

inline void Thread::CloseThread()
{
	if(m_hThread)
	{
		BOOL bHandleClosed = CloseHandle(m_hThread);

		if(!bHandleClosed)
		{
			OutputDebugString("Failed To Close Handle (%p), Error Number: %d", GetLastError());
		}

		m_hThread = nullptr;
	}
}

inline void Thread::ExitThread()
{
	_endthreadex(THREAD_KILL_RET_VAL);
}

void Thread::Init(ThreadFunc pThreadFunc, void* pArgs, bool bAutoStartThread /*= true*/, ThreadFinishCallback pFinishedCallback /*= nullptr*/)
{
	Assert(pThreadFunc, "Invalid Thread Function When Initializing New Thread!");

	if(Verify(!m_hThread, "Thread Has Already Been Started!"))
	{
		m_pThreadFunc = pThreadFunc;

		m_pFinishedCallback = pFinishedCallback;

		if(bAutoStartThread)
			StartThread();
	}
}

bool Thread::StartThread()
{
	bool bThreadStarted = false;

	if(Verify(!m_hThread, "Thread is Already Active, Start Thread will notdo anything."))
	{
		if(Verify(m_pThreadFunc, "No Function Set for the starting thread"))
		{
			m_hThread = (HANDLE)_beginthreadex(NULL, 0, (unsigned int(__stdcall*)(void*))m_pThreadFunc, m_pArgs, 0, NULL);
		}

		return true;
	}

	return bThreadStarted;
}

void Thread::KillThread()
{
	if(m_hThread)
	{
		ExitThread();
		CloseThread();
	}
}