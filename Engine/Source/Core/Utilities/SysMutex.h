#ifndef SYSMUTEX_H
#define SYSMUTEX_H
#include "SysSyncObject.h"

class SysMutex : public SysSyncObject
{
private:

	void InitMutex()
	{
		if(!m_pHandle)
			m_pHandle = CreateMutex(NULL, FALSE, NULL);
	}

public:
	SysMutex() { InitMutex(); }
	~SysMutex() { CloseMutex(); }

	void CloseMutex()
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
		{
			ReleaseMutex(m_pHandle);
		}
	}
};

#endif // SYSMUTEX_H