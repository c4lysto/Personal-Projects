#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <Windows.h>
#include <tchar.h>

class DebugWindow
{
private:
	static HANDLE m_hOutput;

public:
	static bool Initialize();

	static BOOL Shutdown()
	{return FreeConsole();}
};

#endif