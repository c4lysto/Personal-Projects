#ifndef	THREADGROUP_H
#define THREADGROUP_H

#include <list>
using std::list;

#include "Thread.h"
#include "SysSyncObject.h"

class ThreadGroup
{
private:
	SysSyncObjectMultiple m_GroupSyncObject;
	list<Thread*> m_lThreads;

public:
	ThreadGroup();
	~ThreadGroup();

	void Shutdown();

	void AddThread(Thread* pThread);

	void PostWork(ThreadProc pProc, void* pArgs, ThreadFinishCallback pFinishedCallback = nullptr);
};

#endif // THREADPOOL_H