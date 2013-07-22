#include <windows.h>
#include <iostream>
#include <conio.h>
#include <thread>

// Thread debugging helper prototype
void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName );

// BEGIN DEMO
void ThreadA()
{
#ifdef _DEBUG
	SetThreadName( -1, "Thread A" );
#endif
	char buffer[256];
	for(unsigned int i = 0; i < 10; ++i)
	{
		sprintf_s(buffer, 256, "Thread A: Working on item %d of 10.\n", i+1);
		std::cout << buffer;	
		Sleep(500); // simulate thread delay
	}
	std::cout << "Thread A: Work Complete! Exiting Thread.\n";
}

void ThreadB()
{
#ifdef _DEBUG
	SetThreadName( -1, "Thread B" );
#endif
	char buffer[256];
	for(unsigned int i = 0; i < 100; ++i)
	{
		sprintf_s(buffer, 256, "Thread B: Working on item %d of 100.\n", i+1);
		std::cout << buffer;
		Sleep(100);  // simulate thread delay
	}
	std::cout << "Thread B: Work Complete! Exiting Thread.\n";
}

int main(void)
{
	std::cout << "Begin Threaded Application:\n"; 

	std::cout << "Main Thread: Launch Thread A.\n";

	std::thread threadA(ThreadA);

	std::cout << "Main Thread: Launch Thread B.\n";

	std::thread threadB(ThreadB);

	std::cout << "Main Thread: Performing Work.\n";

	char buffer[256];
	for(unsigned int i = 0; i < 50; ++i)
	{
		sprintf_s(buffer, 256, "Main Thread: Working on item %d of 50.\n", i+1);
		std::cout << buffer;
		Sleep(150);  // simulate thread delay
	}

	std::cout << "Main Thread: Work Complete.\n";

	std::cout << "Main Thread: Waiting for coincident threads to complete.\n";

	if(threadA.joinable()) threadA.join();
	if(threadB.joinable()) threadB.join();

	std::cout << "End Threaded Application: (press any key)\n"; 
	_getch();
	return 0;
};
// END DEMO

// HANDY THREAD NAMING FUNCTION FOR DEBUGGING (see a threads name in the debugger)
typedef struct tagTHREADNAME_INFO
{
    DWORD dwType;     // must be 0x1000
    LPCSTR szName;    // pointer to name (in user address space)
    DWORD dwThreadID; // thread ID (-1 = caller thread)
    DWORD dwFlags;    // reserved for future use, must be zero
} THREADNAME_INFO;
// Call this in the thread you wish to label
void SetThreadName( DWORD dwThreadID, LPCSTR szThreadName )
{
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException( 0x406D1388, 0,
                    sizeof(info) / sizeof(DWORD),
		    (const ULONG_PTR*)&info );
    }
    __except( EXCEPTION_CONTINUE_EXECUTION ) {
    }
}
// Example usage:
///SetThreadName(-1, "Main thread");