#ifndef SYSSYNCOBJECT_H
#define SYSSYNCOBJECT_H
#include "UtilitiesInclude.h"

class SysSyncObject
{
	friend class SysSyncObjectMultiple;

protected:
	void* m_pHandle;

public:
	SysSyncObject();
	SysSyncObject(void* pWaitHandle);
	~SysSyncObject();

	SysSyncObject& operator=(const SysSyncObject& rhs)
	{
		if(this != &rhs)
		{
			m_pHandle = rhs.m_pHandle;
		}
		return *this;
	}

	void SetWaitHandle(void* pWaitHandle);

	void Wait(unsigned int nWaitTimeInMS = INFINITE);

	// Returns True if the Object is already signaled
	bool TryWait(unsigned int nWaitTimeInMS = 0);
};

class SysSyncObjectMultiple
{
private:
#ifdef WIN32
	static const size_t MAX_WAIT_HANDLES = MAXIMUM_WAIT_OBJECTS;
#else
	static const size_t MAX_WAIT_HANDLES = 64;
#endif // WIN32

private:
	int m_nNumWaitHandles;
	void* m_pHandles[MAX_WAIT_HANDLES];

public:
	SysSyncObjectMultiple();
	~SysSyncObjectMultiple();

	void AddSyncObject(const SysSyncObject& hWaitObject);
	void RemoveSyncObject(const SysSyncObject& hWaitHandle);

	void ClearWaitHandles();

	void Wait();
	bool WaitAny();
};

#endif // SYSSYNCOBJECT_H