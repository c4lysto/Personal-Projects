#ifndef SYSSEMAPHORE_H
#define SYSSEMAPHORE_H
#include "SysSyncObject.h"

// Semaphore Can Only Be Created Via SysCreateSemaphore(int)
// Semaphore Must Be Closed Via SysCloseSemaphore(SysSemaphore&)
class SysSemaphore : public SysSyncObject
{
public:
	SysSemaphore() {}
	~SysSemaphore() {}

	void Signal(int nSignalCount = 1);

public:

	friend SysSemaphore SysCreateSemaphore(int nInitialCount = 0);
	friend void SysCloseSemaphore(SysSemaphore& sysSemaphore);
};

#endif // SYSSEMAPHORE_H