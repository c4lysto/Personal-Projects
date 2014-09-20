#ifndef WAITOBJECT_H
#define WAITOBJECT_H
#include "UtilitiesInclude.h"

class WaitObject
{
	friend class WaitObjectMultiple;

private:
	void* m_hWaitHandle;
	bool m_bLocallyManaged : 1;

public:
	WaitObject();
	WaitObject(void* hWaitHandle);
	~WaitObject();

	void CreateWaitHandle(int nMaxLockCount = INT_MAX);
	void SetWaitHandle(void* hWaitHandle);

	void CloseWaitHandle();

	void Wait();

	// Returns True if the Object is already signaled
	bool TryWait();

	void Signal(int nSignalCount = 1);
};

class WaitObjectMultiple
{
private:
#ifdef WIN32
	static const size_t MAX_WAIT_HANDLES = MAXIMUM_WAIT_OBJECTS;
#else
	static const size_t MAX_WAIT_HANDLES = 64;
#endif // WIN32

private:
	int m_nNumWaitHandles;
	void* m_hWaitHandles[MAX_WAIT_HANDLES];

public:
	WaitObjectMultiple();
	~WaitObjectMultiple();

	void AddWaitObject(const WaitObject& hWaitObject);
	void RemoveWaitObject(const WaitObject& hWaitHandle);

	void ClearWaitHandles();

	void Wait();
	bool WaitAny();
};

#endif // WAITOBJECT_H