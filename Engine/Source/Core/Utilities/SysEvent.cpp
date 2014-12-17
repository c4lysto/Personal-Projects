#include "SysEvent.h"

void SysEvent::Signal()
{
	if(m_pHandle)
		SetEvent(m_pHandle);
}

void SysEvent::Reset()
{
	if(m_pHandle)
		ResetEvent(m_pHandle);
}

SysEvent SysCreateEvent(bool bManuallyManaged /*= false*/, bool bInitialState /*= false*/)
{
	SysEvent sysEvent;

	void* pEvent = CreateEvent(false, bManuallyManaged, bManuallyManaged ? bInitialState : false, nullptr);

	if(pEvent)
		sysEvent.m_pHandle = pEvent;
#if __ASSERT
	else
		DisplayDebugString("Failed To Create Event!");
#endif // __ASSERT

	return sysEvent;
}

void SysCloseEvent(SysEvent& sysEvent)
{
	if(sysEvent.m_pHandle)
	{
		CloseHandle(sysEvent.m_pHandle);
		sysEvent.m_pHandle = nullptr;
	}
}