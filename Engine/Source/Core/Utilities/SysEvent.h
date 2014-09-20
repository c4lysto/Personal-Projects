#ifndef SYSEVENT_H
#define SYSEVENT_H
#include "SysSyncObject.h"

class SysEvent : public SysSyncObject
{
private:

	void InitEvent()
	{
		if(!m_pHandle)
			m_pHandle = CreateEvent(NULL, FALSE, FALSE, NULL);
	}

public:
	SysEvent() { InitEvent(); }
	~SysEvent() { CloseEvent(); }

	void CloseEvent()
	{
		if(m_pHandle)
		{
			CloseHandle(m_pHandle);
			m_pHandle = nullptr;
		}
	}

	void Signal()
	{
		if(m_pHandle)
			SetEvent(m_pHandle);
	}

	void Reset()
	{
		if(m_pHandle)
			ResetEvent(m_pHandle);
	}
};

#endif // SYSEVENT_H