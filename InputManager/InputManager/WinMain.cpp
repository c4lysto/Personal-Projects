#include <Windows.h>
#include <stdio.h>
#include <DbgHelp.h>
#pragma comment(lib,"dbghelp.lib")
#include <tchar.h>
//#include <DbgHelp.h>
//#include "../../Visual Leak Detector/include/vld.h"

// used for multithreading
#include <process.h>

const TCHAR* CLASS_NAME = _T("RM_InputManager");
const TCHAR* WINDOW_NAME = _T("InputManager");

const bool WINDOWED = true;

const int WINDOW_WIDTH = 1024;
const int WINDOW_HEIGHT = 768;

#define  ALLOW_MUL_WINDOW_INSTANCES 0

#if  ALLOW_MUL_WINDOW_INSTANCES
BOOL AlreadyRunning();
#endif

BOOL RegisterWindow(HINSTANCE hInstance);
HWND MakeWindow(HINSTANCE hInstance);
LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LONG WINAPI WriteDumpFile(_EXCEPTION_POINTERS* exceptions);

#define RAW_INPUT_BUFFER_SIZE 48

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(-);

#ifndef _DEBUG
	SetUnhandledExceptionFilter(WriteDumpFile);
#endif

#if  ALLOW_MUL_WINDOW_INSTANCES
	if(!hPrevInstance && AlreadyRunning())
		return FALSE;
#endif

	if(!RegisterWindow(hInstance))
		return FALSE;

	HWND hWnd = MakeWindow(hInstance);

	if(!hWnd)
	{
		int tmp = GetLastError();
		return 0;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	UnregisterClass(CLASS_NAME, hInstance);

	return (int)(msg.wParam);
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		// Alt+F4
	case WM_CLOSE:
		{
			DestroyWindow(hWnd);
		}
		break;

		// losing and gaining focus of window
	case WM_ACTIVATE:
		{
			// gaining Focus
			if(LOWORD(wParam) != WA_INACTIVE)
			{
				
			}
			// losing Focus
			else
			{

			}
		}
		break;

		// Window Creation
	case WM_CREATE:
		{
			RAWINPUTDEVICE inputDevices[2];

			// mouse
			inputDevices[0].usUsagePage = 1;
			inputDevices[0].usUsage = 6;
			inputDevices[0].dwFlags = 0;
			inputDevices[0].hwndTarget = hWnd;

			// keyboard
			inputDevices[1].usUsagePage = 1;
			inputDevices[1].usUsage = 2;
			inputDevices[1].dwFlags = 0;
			inputDevices[1].hwndTarget = hWnd;

			if(FALSE == RegisterRawInputDevices(inputDevices, 2, sizeof(RAWINPUTDEVICE)))
				MessageBox(hWnd, L"Failed To Register Raw Input Devices", L"", MB_OK | MB_ICONWARNING);
		}
		break;

	case WM_INPUT:
		{
			UINT bufferSize = RAW_INPUT_BUFFER_SIZE;
			BYTE inputBuffer[RAW_INPUT_BUFFER_SIZE];

			GetRawInputData((HRAWINPUT)lParam, RID_INPUT, inputBuffer, &bufferSize, sizeof(RAWINPUTHEADER));

			RAWINPUT* raw = (RAWINPUT*)inputBuffer;
		}
		break;

	case WM_DESTROY:
		{
			PostQuitMessage(0);
		}
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

BOOL RegisterWindow(HINSTANCE hInstance)
{
	WNDCLASSEX window = {};

	window.cbSize			= sizeof(WNDCLASSEX);
	window.style			= CS_HREDRAW | CS_VREDRAW;
	window.lpfnWndProc		= WindowProc;
	window.cbClsExtra		= 0;
	window.cbWndExtra		= 0;
	window.hInstance		= hInstance;
	window.hCursor			= NULL;// LoadCursor(NULL, IDC_ARROW);
	window.hbrBackground	= (HBRUSH)GetStockObject(BLACK_BRUSH);
	window.hIconSm			= NULL;
	window.hIcon			= NULL;
	window.lpszMenuName		= NULL;
	window.lpszClassName	= CLASS_NAME;

	return RegisterClassEx(&window);
}

HWND MakeWindow(HINSTANCE hInstance)
{
	// Setup window style flags
	DWORD dwStyleFlags = WS_VISIBLE;

	if(!WINDOWED)
		// Window without border
		dwStyleFlags |= WS_POPUP;
	else
		// Window with border
		dwStyleFlags |= WS_OVERLAPPEDWINDOW;

	RECT rWindow = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};
	
	AdjustWindowRectEx(&rWindow, dwStyleFlags, FALSE, WS_EX_APPWINDOW);
	
	// Calculate the width/height of that window's dimensions
	int nWindowWidth	= rWindow.right - rWindow.left;
	int nWindowHeight	= rWindow.bottom - rWindow.top;

	//	Create the window
	return CreateWindowEx(WS_EX_APPWINDOW, CLASS_NAME, WINDOW_NAME,	dwStyleFlags,												
						  (GetSystemMetrics(SM_CXSCREEN) / 2) - (nWindowWidth / 2),		
						  (GetSystemMetrics(SM_CYSCREEN) / 2) - (nWindowHeight / 2),	
						  nWindowWidth,	nWindowHeight, NULL, NULL, hInstance, NULL);
}

#if ALLOW_MUL_WINDOW_INSTANCES
BOOL AlreadyRunning()
{
	HWND hWnd = FindWindow(CLASS_NAME, WINDOW_NAME);

	// if window was found
	if(hWnd)
	{
		// if it was minimized
		if(IsIconic(hWnd))
			// Restore Window
			ShowWindow(hWnd, SW_RESTORE);

		// bring window to the front
		SetForegroundWindow(hWnd);

		return TRUE;
	}

	return FALSE;
}
#endif

LONG WINAPI WriteDumpFile(_EXCEPTION_POINTERS* exceptions)
{
	HANDLE hFile = CreateFile(L"dumpfile.mdmp", GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL );

	if (hFile != INVALID_HANDLE_VALUE)
	{
		_MINIDUMP_EXCEPTION_INFORMATION ExInfo;

		ExInfo.ThreadId = GetCurrentThreadId();
		ExInfo.ExceptionPointers = exceptions;
		ExInfo.ClientPointers = NULL;
		MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpWithFullMemory, &ExInfo, NULL, NULL);
		CloseHandle(hFile);
	}

	return EXCEPTION_EXECUTE_HANDLER;
}