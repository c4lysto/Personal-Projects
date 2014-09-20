#ifndef SYSSEMAPHORE_H
#define SYSSEMAPHORE_H
#include "SysSyncObject.h"

class SysSemaphore : public SysSyncObject
{
private:

	void InitSemaphore(int nInitCount)
	{
		if(!m_pHandle)
			m_pHandle = CreateSemaphore(NULL, nInitCount, LONG_MAX, NULL);
	}

public:
	SysSemaphore(int nInitCount = 0) {InitSemaphore(nInitCount);}
	~SysSemaphore() { CloseSemaphore(); }

	void CloseSemaphore()
	{
		if(m_pHandle)
		{
			CloseHandle(m_pHandle);
			m_pHandle = nullptr;
		}
	}

	void Signal(int nSignalCount = 1)
	{
		if(m_pHandle)
			ReleaseSemaphore(m_pHandle, nSignalCount, NULL);
	}
};

#endif // SYSSEMAPHORE_H