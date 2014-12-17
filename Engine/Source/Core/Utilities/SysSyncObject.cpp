#include "SysSyncObject.h"

SysSyncObject::SysSyncObject() : m_pHandle(nullptr)
{	
}

SysSyncObject::SysSyncObject(void* pWaitHandle) : m_pHandle(nullptr)
{
	SetHandle(pWaitHandle);
}

SysSyncObject::~SysSyncObject()
{
	m_pHandle = nullptr;
}

void SysSyncObject::SetHandle(void* pWaitHandle)
{
	if(!m_pHandle && pWaitHandle)
	{
		m_pHandle = pWaitHandle;
	}
}

void SysSyncObject::Wait(unsigned int nWaitTimeInMS /*= INFINITE*/)
{
	if(m_pHandle)
	{
		WaitForSingleObject(m_pHandle, nWaitTimeInMS);
	}
}

bool SysSyncObject::TryWait(unsigned int nWaitTimeInMS /*= 0*/)
{
	if(m_pHandle)
	{
		if(WaitForSingleObject(m_pHandle, nWaitTimeInMS) == WAIT_OBJECT_0)
			return true;
	}

	return false;
}

SysSyncObjectMultiple::SysSyncObjectMultiple()
{
	ClearWaitHandles();
}

SysSyncObjectMultiple::~SysSyncObjectMultiple()
{
	ClearWaitHandles();
}

void SysSyncObjectMultiple::AddSyncObject(const SysSyncObject& hSysSyncObject)
{
	if(hSysSyncObject.m_pHandle)
	{
		if(m_nNumWaitHandles < MAX_WAIT_HANDLES)
		{
			m_pHandles[m_nNumWaitHandles++] = hSysSyncObject.m_pHandle;
		}
#if __ASSERT
		else
		{
			Assert(false, "Wait Object Multiple - Max Wait Object Limit Reached!");
		}
#endif // __ASSERT
	}
}

void SysSyncObjectMultiple::RemoveSyncObject(const SysSyncObject& hSysSyncObject)
{
	for(int i = 0; i < m_nNumWaitHandles; ++i)
	{
		if(m_pHandles[i] == hSysSyncObject.m_pHandle)
		{
			memmove(m_pHandles+i, m_pHandles+i+1, sizeof(*m_pHandles)*((--m_nNumWaitHandles) - i));
			return;
		}
	}
}

void SysSyncObjectMultiple::ClearWaitHandles()
{
	memset(m_pHandles, 0, sizeof(*m_pHandles)*MAX_WAIT_HANDLES);
	m_nNumWaitHandles = 0;
}

void SysSyncObjectMultiple::Wait()
{
	if(*m_pHandles)
	{
		WaitForMultipleObjects(m_nNumWaitHandles, m_pHandles, TRUE, INFINITE);
	}
}

bool SysSyncObjectMultiple::WaitAny()
{
	if(*m_pHandles)
	{
		if(WaitForMultipleObjects(m_nNumWaitHandles, m_pHandles, FALSE, INFINITE) < WAIT_ABANDONED)
			return true;
	}
	return false;
}