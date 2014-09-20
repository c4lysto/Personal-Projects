#ifndef THREAD_H
#define THREAD_H

#include "SysSemaphore.h"

typedef int(*ThreadProc)(void*);
typedef void(*ThreadFinishCallback)(int, void*);

#define THREAD_KILL_RET_VAL (0xCDCDCDCDU)

class Thread
{
private:
	struct ThreadArgs
	{
		void* m_pArgs;
		ThreadProc m_pThreadFunc;
		ThreadFinishCallback m_pFinishedCallback;
		SysSemaphore m_JobWaitObject;

		ThreadArgs() : m_pArgs(nullptr), m_pThreadFunc(nullptr), m_pFinishedCallback(nullptr)
		{
		}

		~ThreadArgs()
		{
		}

		inline void Reset()
		{
			m_pArgs = nullptr;
			m_pThreadFunc = nullptr;
			m_pFinishedCallback = nullptr;
		}
	};

private:
	void* m_hThread;
	SysSemaphore m_ThreadWaitObject;
	ThreadArgs m_ThreadArgs;
	
	bool StartThread();
	void CloseThread();
	void ExitThread();

	static unsigned int __stdcall DefaultThreadProc(void* pArgs);

public:
	Thread();
	~Thread();

	// Undefined to avoid bad things.
	Thread(const Thread& rhs);// = delete;
	Thread& operator=(const Thread& rhs);// = delete;

	SysSyncObject GetThreadWaitObject() const {return m_ThreadWaitObject;}
	void* GetThreadHandle() const {return m_hThread;}

	// pProc - Pass NULL to kill the thread
	// pArgs - This memory should be allocated and deleted properly outside of the function
	void AssignTask(ThreadProc pProc, void* pArgs, ThreadFinishCallback pFinishedCallback = nullptr, bool bAutoSignalWork = true);

	// Will only signal Work if the Thread Proc Is Valid, see AssignTask
	void SignalWork();

	void KillThread();
};

#endif //THREAD_H