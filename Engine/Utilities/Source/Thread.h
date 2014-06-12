#ifndef THREAD_H
#define THREAD_H

#include <thread>

class Thread
{
private:
	HANDLE m_hThread;

public:
	Thread();
	~Thread();

	// Undefined to avoid bad things.
	Thread(const Thread& rhs);
	Thread& operator=(const Thread& rhs);
};

#endif //THREAD_H