#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <Windows.h>
#include "Input.h"

#include <map>
using std::map;
using std::pair;

class InputManager
{
private:
	unsigned char m_ucMofifiers;
	unsigned char m_ucConnectedDevices;

	//map<IInputEvent, pair<IInputListener*, INPUT_EVENT_HANDLER>> m_mCurrentInputEvents;
	map<IInputEvent, IInputListener*> m_mInputListeners;

	void CreateKeyboardEvent(PRAWINPUT pInput);
	void PostMouseEvent(PRAWINPUT pInput);

	void PostInputEvent(const IInputEvent& inputEvent);

public:
	InputManager();
	~InputManager();

	bool Initialize(HWND hWnd = NULL);
	void ProcessInput(WPARAM wParam, LPARAM lParam);
	void GetAttachedDevices();

	void SetDeviceChange(WPARAM wParam, LPARAM lParam);

	void RegisterEventListener(IInputListener* pListener, const IInputEvent& inputEvent, INPUT_EVENT_HANDLER fpEventHandler);
};

#endif