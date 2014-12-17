#include "SysMutex.h"

void SysMutex::Signal()
{
	if(m_pHandle)
	{
		ReleaseMutex(m_pHandle);
	}
}

SysMutex SysCreateMutex(bool bInitialOwner /*= false*/)
{
	SysMutex sysMutex;

	void* pMutex = CreateMutex(nullptr, bInitialOwner, nullptr);

	if(pMutex)
		sysMutex.m_pHandle = pMutex;
#if __ASSERT
	else
		DisplayDebugString("Failed To Create Mutex!");
#endif // __ASSERT

	return sysMutex;
}

void SysCloseMutex(SysMutex& sysMutex)
{
	if(sysMutex.m_pHandle)
	{
		CloseHandle(sysMutex.m_pHandle);
		sysMutex.m_pHandle = nullptr;
	}
}
