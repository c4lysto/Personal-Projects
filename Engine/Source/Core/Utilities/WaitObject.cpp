#include "WaitObject.h"

WaitObject::WaitObject() : m_hWaitHandle(nullptr) , m_bLocallyManaged(false)
{	
}

WaitObject::WaitObject(void* hWaitHandle) : m_hWaitHandle(nullptr), m_bLocallyManaged(false)
{
	SetWaitHandle(hWaitHandle);
}

WaitObject::~WaitObject()
{
	CloseWaitHandle();
}

void WaitObject::CreateWaitHandle(s32 nMaxLockCount)
{
	if(!m_hWaitHandle)
	{
		if(Verify(nMaxLockCount, "Wait Object - Max Lock Count Cannot Be Zero!"))
		{
			m_hWaitHandle = CreateSemaphore(NULL, 0, nMaxLockCount, NULL);		
			m_bLocallyManaged = true;
		}
	}
}

void WaitObject::SetWaitHandle(void* hWaitHandle)
{
	if(!m_hWaitHandle)
	{
		if(hWaitHandle)
		{
			m_hWaitHandle = hWaitHandle;
			m_bLocallyManaged = false;
		}
	}
}

void WaitObject::CloseWaitHandle()
{
	if(m_hWaitHandle)
	{
		if(m_bLocallyManaged)
		{
			CloseHandle(m_hWaitHandle);
		}
		m_hWaitHandle = nullptr;
		m_bLocallyManaged = false;
	}
}

void WaitObject::Wait()
{
	if(m_hWaitHandle)
	{
		WaitForSingleObject(m_hWaitHandle, INFINITE);
	}
}

bool WaitObject::TryWait()
{
	if(m_hWaitHandle)
	{
		if(WaitForSingleObject(m_hWaitHandle, 0) == WAIT_OBJECT_0)
			return true;
	}

	return false;
}

void WaitObject::Signal(s32 nSignalCount /*= 1*/)
{
	ReleaseSemaphore(m_hWaitHandle, nSignalCount, NULL);
}

WaitObjectMultiple::WaitObjectMultiple()
{
	ClearWaitHandles();
}

WaitObjectMultiple::~WaitObjectMultiple()
{
	ClearWaitHandles();
}

void WaitObjectMultiple::AddWaitObject(const WaitObject& hWaitObject)
{
	if(hWaitObject.m_hWaitHandle)
	{
		if(m_nNumWaitHandles < MAX_WAIT_HANDLES)
		{
			m_hWaitHandles[m_nNumWaitHandles++] = hWaitObject.m_hWaitHandle;
		}
#if __ASSERT
		else
		{
			Assert(false, "Wait Object Multiple - Max Wait Object Limit Reached!");
		}
#endif // __ASSERT
	}
}

void WaitObjectMultiple::RemoveWaitObject(const WaitObject& hWaitObject)
{
	for(int i = 0; i < m_nNumWaitHandles; ++i)
	{
		if(m_hWaitHandles[i] == hWaitObject.m_hWaitHandle)
		{
			memmove(m_hWaitHandles+i, m_hWaitHandles+i+1, sizeof(*m_hWaitHandles)*((--m_nNumWaitHandles) - i));
			return;
		}
	}
}

void WaitObjectMultiple::ClearWaitHandles()
{
	memset(m_hWaitHandles, 0, sizeof(*m_hWaitHandles)*MAX_WAIT_HANDLES);
	m_nNumWaitHandles = 0;
}

void WaitObjectMultiple::Wait()
{
	if(*m_hWaitHandles)
	{
		WaitForMultipleObjects(m_nNumWaitHandles, m_hWaitHandles, TRUE, INFINITE);
	}
}

bool WaitObjectMultiple::WaitAny()
{
	if(*m_hWaitHandles)
	{
		if(WaitForMultipleObjects(m_nNumWaitHandles, m_hWaitHandles, FALSE, INFINITE) < WAIT_ABANDONED)
			return true;
	}
	return false;
}