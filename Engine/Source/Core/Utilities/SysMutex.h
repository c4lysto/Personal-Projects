#ifndef SYSMUTEX_H
#define SYSMUTEX_H
#include "SysSyncObject.h"

// Mutex Can Only Be Created Via SysCreateMutex(bool)
// Mutex Must Be Closed Via SysMutex::CloseMutex(SysMutex&)
class SysMutex : public SysSyncObject
{
public:
	SysMutex() {}
	~SysMutex() {}

	void Signal();

public:

	friend SysMutex SysCreateMutex(bool bInitialOwner = false);
	friend void SysCloseMutex(SysMutex& sysMutex);
};

#endif // SYSMUTEX_H