#ifndef	THREADPOOL_H
#define THREADPOOL_H

#include <vector>
using std::vector;

#include <queue>
using std::queue;

#include "SysThread.h"
#include "SysSemaphore.h"
#include "CriticalSection.h"

class ThreadPool
{
private:
	struct JobArgs
	{
		SysThreadProc m_pProc;
		void* m_pArgs;
		eThreadPriority m_ePriority;

		JobArgs() {}
		JobArgs(SysThreadProc pProc, void* pArgs, eThreadPriority ePriority) :
			m_pProc(pProc), m_pArgs(pArgs), m_ePriority(ePriority) {}

		/*const JobArgs& operator=(const JobArgs& rhs)
		{
			if(this != &rhs)
			{
				m_pProc = rhs.m_pProc; 
				m_pArgs = rhs.m_pArgs;
				m_ePriority = rhs.m_ePriority;
			}
			return *this;
		}*/
	};

private:
	SysSyncObjectMultiple m_GroupSyncObject;
	SysSemaphore m_JobSemaphore;
	CriticalSection m_JobCS;
	vector<SysThread*> m_vThreads;
	queue<JobArgs> m_qJobs;

	void AddThread(SysThread* pThread);

	static void WorkerThreadProc(void* pArgs);

	JobArgs GetWork();

public:
	ThreadPool();
	~ThreadPool();

	// Optional Call
	void Init(size_t poolSize);

	void Shutdown();

	void AddWork(SysThreadProc pProc, void* pArgs, eThreadPriority ePriority = THREAD_PRIO_NORMAL);
};

#endif // THREADPOOL_H