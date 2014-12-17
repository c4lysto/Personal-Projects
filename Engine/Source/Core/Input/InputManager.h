#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Windows.h>
#include "Input.h"
#include "../Utilities/SysThread.h"
#include "../Utilities/CriticalSection.h"
#include "../Utilities/SysHook.h"
#include "../Utilities/SysEvent.h"

#include <map>
using std::map;
using std::pair;

#include <list>
using std::list;

#include <queue>
using std::queue;

class InputManager
{
	typedef list<InputEvent*> InputCurrentEventContainer;
	typedef map<InputEvent, InputCallbackfn> InputCallbackContainer;

private:
	struct PendingInputEvent
	{
		WPARAM wParam;
		LPARAM lParam;
	};
	typedef queue<PendingInputEvent> PendingInputEventContainer;


private:
	void* m_WindowHandle;
	SysThread m_InputThread;
	SysEvent m_NewInputEvent;
	CriticalSection m_PendingInputCS;
	CriticalSection m_CurrentInputCS;
	SysHook m_InputHook;

	unsigned char m_ucModifiers;
	unsigned char m_ucConnectedDevices;

	PendingInputEventContainer m_PendingInputEvents;
	InputCurrentEventContainer m_lCurrentEvents;
	InputCallbackContainer m_mInputCallbacks;

	InputEvent* GetCurrentInputEvent(const InputEvent& inputEvent) const;

	void HandleModifierInput(const InputEvent& inputEvent);
	void SetInputModifier(EInputModifier eModifier, bool bSet);

	void CreateKeyboardEvent(PRAWINPUT pInput);
	void CreateMouseEvent(PRAWINPUT pInput);

	void PostButtonEvent(InputEvent& inputEvent, bool bIsDown);
	void PostMouseButtonEvent(RAWMOUSE& rwMouse, InputMouseButtonEvent& newEvent);
	void PostInputEvent(const InputEvent* inputEvent);

	void ClearAllCurrentInput();

	void InputThreadProc(void* pArgs);

#if DEBUG
	void DisplayCurrentEvents();
#endif

public:
	InputManager();
	~InputManager();

	void Shutdown();

	bool Initialize(HWND hWnd = NULL);
	void ProcessInputEvent(WPARAM wParam, LPARAM lParam);
	void AddInputRequest(WPARAM wParam, LPARAM lParam);
	void ProcessInput();
	void GetAttachedDevices();

	void OnWindowFocusActivate();
	void OnWindowFocusDeactivate();
	void SetDeviceChange(WPARAM wParam, LPARAM lParam);

	void RegisterEventCallback(InputEvent& inputEvent, InputCallbackfn& fpEventHandler, bool bOverrideCallback = false);

	Vec2i GetMousePosition();

public:
	static LRESULT __stdcall InputHookCallback(int code, WPARAM wParam, LPARAM lParam);
};

extern InputManager g_InputManager;

#endif