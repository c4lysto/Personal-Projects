#ifndef THREAD_H
#define THREAD_H

#include <windef.h>

typedef int(*ThreadFunc)(void*);
typedef void(*ThreadFinishCallback)(int);

#define THREAD_KILL_RET_VAL (0xCDCDCDCDU)

class Thread
{
private:
	HANDLE m_hThread;
	void* m_pArgs;
	ThreadFunc m_pThreadFunc;
	ThreadFinishCallback m_pFinishedCallback;

	void CloseThread();
	void ExitThread();

public:
	Thread();
	~Thread();

	// Undefined to avoid bad things.
	Thread(const Thread& rhs) = delete;
	Thread& operator=(const Thread& rhs) = delete;

	HANDLE GetThreadHandle() const {return m_hThread;}

	// pArgs - dynamically allocated memory, will be deleted after the thread is finished
	void Init(ThreadFunc pThreadFunc, void* pArgs, bool bAutoStartThread = true, ThreadFinishCallback pFinishedCallback = nullptr);

	bool StartThread();
	void KillThread();
};

#endif //THREAD_H