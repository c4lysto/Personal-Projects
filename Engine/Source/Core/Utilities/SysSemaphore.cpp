#include "SysSemaphore.h"

void SysSemaphore::Signal(int nSignalCount /*= 1*/)
{
	if(m_pHandle)
		ReleaseSemaphore(m_pHandle, nSignalCount, NULL);
}

SysSemaphore SysCreateSemaphore(int nInitialCount /*= 0*/)
{
	SysSemaphore sysSemaphore;

	void* pSemaphore = CreateSemaphore(nullptr, nInitialCount, LONG_MAX, nullptr);

	if(pSemaphore)
		sysSemaphore.m_pHandle = pSemaphore;
#if __ASSERT
	else
		DisplayDebugString("Failed To Create Semaphore!");
#endif // __ASSERT

	return sysSemaphore;
}

void SysCloseSemaphore(SysSemaphore& sysSemaphore)
{
	if(sysSemaphore.m_pHandle)
	{
		CloseHandle(sysSemaphore.m_pHandle);
		sysSemaphore.m_pHandle = nullptr;
	}
}