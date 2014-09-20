#include "ThreadGroup.h"

ThreadGroup::ThreadGroup()
{

}

ThreadGroup::~ThreadGroup()
{
	Shutdown();
}

void ThreadGroup::AddThread(Thread* pThread)
{
	if(pThread)
	{
		for(list<Thread*>::const_iterator iter = m_lThreads.begin(); iter != m_lThreads.end(); ++iter)
		{
			if(*iter == pThread)
				return;
		}

		m_lThreads.push_back(pThread);
	}
}

void ThreadGroup::Shutdown()
{
	m_GroupSyncObject.Wait();
}