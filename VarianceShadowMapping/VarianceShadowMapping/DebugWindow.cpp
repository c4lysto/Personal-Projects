#include "DebugWindow.h"
#include <iostream>
#include <io.h>
#include <fcntl.h>

HANDLE DebugWindow::m_hOutput = nullptr;

bool DebugWindow::Initialize()
{
	if(!AllocConsole())
		return false;

	int conHandle;
	FILE* pFile;

	m_hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	conHandle = _open_osfhandle((intptr_t)m_hOutput, _O_TEXT);
    pFile = _fdopen(conHandle, "w");
    *stdout = *pFile;
    setvbuf(stdout, NULL, _IONBF, 0);

	if(!m_hOutput)
		return false;

	return true;
}