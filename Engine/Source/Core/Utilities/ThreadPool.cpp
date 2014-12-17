#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	m_JobSemaphore = SysCreateSemaphore();
}

ThreadPool::~ThreadPool()
{
	Shutdown();
}

void ThreadPool::AddThread(SysThread* pThread)
{
	if(pThread)
	{
		for(vector<SysThread*>::const_iterator iter = m_vThreads.begin(); iter != m_vThreads.end(); ++iter)
		{
			if(*iter == pThread)
				return;
		}

		m_vThreads.push_back(pThread);
		m_GroupSyncObject.AddSyncObject(*pThread);

		pThread->StartThread(WorkerThreadProc, this, THREAD_PRIO_NORMAL);
	}
}

void ThreadPool::WorkerThreadProc(void* pArgs)
{
	if(pArgs)
	{
		ThreadPool* pThreadPool = (ThreadPool*)pArgs;
		JobArgs jobArgs;
		
		if(pThreadPool)
		{
			while(true)
			{
				jobArgs = pThreadPool->GetWork();

				if(!jobArgs.m_pProc.IsNull())
				{
					SysSetThreadPriority(jobArgs.m_ePriority);

					(jobArgs.m_pProc)(jobArgs.m_pArgs);

					SysSetThreadPriority(THREAD_PRIO_NORMAL);
				}
				else
				{
					break;
				}
			}
		}
	}
}

ThreadPool::JobArgs ThreadPool::GetWork()
{
	m_JobSemaphore.Wait();

	LocalCriticalSection jobCS(m_JobCS);

	JobArgs pJob = m_qJobs.front();
	m_qJobs.pop();

	return pJob;
}

void ThreadPool::Init(size_t poolSize)
{
	// Pool Has Already Been Initialized, Don't Be Stupid
	if(m_vThreads.size() == 0)
	{
		for(size_t i = 0; i < poolSize; ++i)
		{
			AddThread(new SysThread);
		}
	}
}

void ThreadPool::Shutdown()
{
	// Add Empty Work in the work Queue to trigger worker threads to stop
	for(size_t i = 0; i < m_vThreads.size(); ++i)
	{
		AddWork(nullptr, nullptr);
	}

	for(size_t i = 0; i < m_vThreads.size(); ++i)
	{
		if(m_vThreads[i])
		{
			m_vThreads[i]->EndThread();
		}
	}

	// Wait For All The Threads To Finish
	m_GroupSyncObject.Wait();

	for(size_t i = 0; i < m_vThreads.size(); ++i)
	{
		SAFE_DELETE(m_vThreads[i]);
	}

	m_vThreads.clear();

	SysCloseSemaphore(m_JobSemaphore);
}

void ThreadPool::AddWork(SysThreadProc pProc, void* pArgs, eThreadPriority ePriority /*= THREAD_PRIO_NORMAL*/)
{
	LocalCriticalSection jobCS(m_JobCS);

	m_qJobs.push(JobArgs(pProc, pArgs, ePriority));

	m_JobSemaphore.Signal();
}