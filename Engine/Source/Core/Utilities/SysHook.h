#ifndef SYSHOOK_H
#define SYSHOOK_H

#include <Windows.h>
#include "Debugging\DebugHelp.h"

typedef LRESULT (__stdcall *SysHookCallback)(int, WPARAM, LPARAM);

enum eHookType
{
	HOOK_CALLWNDPROC = 4,		// Hooks before the system sends them to the destination window procedure: See "CallWndProc" hook procedure on msdn for details on callback
	HOOK_CALLWNDPROCRET = 12,	// Hook that monitors messages after they have been processed by the destination window procedure.
								// See "CallWndRetProc" hook procedure on msdn for details on callback
	HOOK_CBT = 5,				// Hook procedure that receives notifications useful to a CBT application: See "CBTProc" hook procedure on msdn for details on callback
	HOOK_DEBUG = 9,				// Hook procedure useful for debugging other hook procedures: See "DebugProc" hook procedure on msdn for details on callback
	HOOK_FOREGROUNDIDLE = 11,	// Hook procedure that will be called when the application's foreground thread is about to become idle.
								// See "ForegroundIdleProc" hook procedure on msdn for details on callback
	HOOK_GETMESSAGE = 3,		// Hook procedure that monitors messages posted to a message queue: See "GetMsgProc" hook procedure on msdn for details on callback
	HOOK_JOURNALPLAYBACK = 1,	// Hook procedure that posts messages previously recorded by a HOOK_JOURNALPLAYBACK hook procedure.
								// See "JournalPlaybackProc" hook procedure on msdn for details on callback
	HOOK_JOURNALRECORD = 0,		// Hook procedure that records input messages posted to the system message queue. This hook is useful for recording macros.
								// See "JournalRecordProc" hook procedure on msdn for details on callback
	HOOK_KEYBOARD = 2,			// Hook procedure that monitors keystroke messages: See "KeyboardProc" hook procedure on msdn for details on callback.
	HOOK_LOW_LVL_KEYBOARD = 13,	// Hook procedure that monitors low-level keyboard input events: See "LowLevelKeyboardProc" hook procedure on msdn for details on callback.
	HOOK_MOUSE = 7,				// Hook procedure that monitors mouse messages: See "MouseProc" hook procedure on msdn for details on callback.
	HOOK_LOW_LVL_MOUSE = 14,	// Hook procedure that monitors low-level mouse input events: See "LowLevelMouseProc" hook procedure on msdn for details on callback.
	HOOK_MSGFILTER = -1,		// Hook procedure that monitors messages generated as a result of an input event in a dialog box, message box, menu, or scroll bar:
								// See "MessageProc" hook procedure on msdn for details on callback.
	HOOK_SHELL = 10,			// Hook procedure that receives notifications useful to shell applications: See "ShellProc" hook procedure on msdn for details on callback.
	HOOK_SYSMSGFILTER = 6		// Hook procedure that monitors messages generated as a result of an input event in a dialog box, message box, menu, or scroll bar. 
								// The hook procedure monitors these messages for all applications in the same desktop as the calling thread.
								// See "SysMsgProc" hook procedure on msdn for details on callback.
};

class SysHook
{
private:
	void* m_pHook;
	SysHookCallback m_pHookCallback;

public:
	SysHook();
	~SysHook();

	void Init(eHookType hookType, SysHookCallback hookCallback);
	void Shutdown();
};

#endif // SYSHOOK_H