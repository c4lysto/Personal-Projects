#ifndef SYSTHREAD_H
#define SYSTHREAD_H

#include "SysSyncObject.h"
#include "FunctionPointer.h"

typedef FunctionPointer<void, void*> SysThreadProc;

#define THREAD_KILL_RET_VAL (0xCDCDCDCDU)

enum eThreadPriority
{
	THREAD_PRIO_IDLE = -15,
	THREAD_PRIO_LOW = -2,
	THREAD_PRIO_BELOW_NORMAL = -1,
	THREAD_PRIO_NORMAL = 0,
	THREAD_PRIO_ABOVE_NORMAL = 1,
	THREAD_PRIO_HIGH = 2,
	THREAD_PRIO_CRITICAL = 15
};

class SysThread : public SysSyncObject
{
private:
	struct ThreadArgs
	{
		void* m_pArgs;
		char* m_szThreadName;
		eThreadPriority m_ePriority;
		SysThreadProc m_pThreadFunc;

		ThreadArgs() :
			m_pArgs(nullptr),
			m_szThreadName(nullptr),
			m_ePriority(THREAD_PRIO_NORMAL),
			m_pThreadFunc(nullptr)
		{}
	};

private:
	ThreadArgs m_ThreadArgs;

	void CloseThread();

	static unsigned int __stdcall DefaultThreadProc(void* pArgs);

public:
	SysThread();
	~SysThread();

	// Undefined to avoid bad things.
	SysThread(const SysThread& rhs) = delete;
	SysThread& operator=(const SysThread& rhs) = delete;

	bool StartThread(SysThreadProc pThreadProc, void* pArgs, eThreadPriority = THREAD_PRIO_NORMAL, char* szThreadName = nullptr);

	void EndThread();

	// Only Call This If You Know What You Are Doing!
	void KillThread();
};

// hThread - Pass nullptr to set Calling Thread's Name
void SysSetThreadName(const char* szName, void* hThread = nullptr);

// hThread - Pass nullptr to set Calling Thread's Priority
void SysSetThreadPriority(eThreadPriority eThreadPrio, void* hThread = nullptr);

#endif //THREAD_H