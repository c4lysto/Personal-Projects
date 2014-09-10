#include "InputManager.h"
#include <sstream>
#include <iostream>
#include <string>
#include <tchar.h>
using namespace std;

#define RID_USAGE_PAGE_GENERIC (USHORT(0x01))
#define RID_USAGE_PAGE_GAME (USHORT(0x05)) /*Not Sure Quite What This Means Yet...*/

#define RID_USAGE_MOUSE (USHORT(0x02))
#define RID_USAGE_KEYBOARD (USHORT(0x06))
#define RID_USAGE_GAMEPAD (USHORT(0x05))

#define RID_KEY_UP_FLAG (0x1)

#define RID_ALT_SCANCODE_FLAG (0x2)
#define RID_SCANCODE_OFFSET 6

#define USE_BUFFERED_INPUT 0

InputManager::InputManager() : m_ucMofifiers(0), m_ucConnectedDevices(DID_Mouse | DID_Keyboard)
{

}

InputManager::~InputManager()
{

}

bool InputManager::Initialize(HWND hWnd)
{
	RAWINPUTDEVICE Rid[2];
	ZeroMemory(Rid, sizeof(Rid));

	Rid[0].usUsagePage = RID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = RID_USAGE_MOUSE;
	Rid[0].dwFlags = RIDEV_DEVNOTIFY | RIDEV_NOLEGACY;
	Rid[0].hwndTarget = hWnd;

	Rid[1].usUsagePage = RID_USAGE_PAGE_GENERIC;
	Rid[1].usUsage = RID_USAGE_KEYBOARD;
	Rid[1].dwFlags = RIDEV_DEVNOTIFY | RIDEV_NOLEGACY;
	Rid[1].hwndTarget = hWnd;

	if (FALSE == RegisterRawInputDevices(Rid, sizeof(Rid) / sizeof(*Rid), sizeof(RAWINPUTDEVICE)))
	{
		MessageBox(NULL, _T("Failed To Register Raw Input Devices"), _T(""), MB_OK | MB_ICONERROR);
		return false;
	}

	return true;
}

void InputManager::GetAttachedDevices()
{
	//UINT unBuffSize = 0;

	//if(GetRawInputDeviceList(NULL, &unBuffSize, sizeof(RAWINPUTDEVICELIST)) != 0)
	//{
	//	return;
	//}

	//RAWINPUTDEVICELIST* pInputDevices = new RAWINPUTDEVICELIST[unBuffSize];// (PRAWINPUTDEVICELIST)malloc(unBuffSize);

	//unsigned int unNumDevices = GetRawInputDeviceList(pInputDevices, &unBuffSize, sizeof(RAWINPUTDEVICELIST));

	//if(unNumDevices == (UINT)-1)
	//{
	//	return;
	//}

	//for(unsigned int i = 0; i < unNumDevices; ++i)
	//{
	//	RAWINPUTDEVICELIST& tmp = pInputDevices[i];

	//	int x = 0;
	//}

	//delete[] pInputDevices;
	//g_bCheckAttached = false;

	/*UINT numDevices;
	GetRegisteredRawInputDevices(nullptr, &numDevices, sizeof(RAWINPUTDEVICE));

	RAWINPUTDEVICE* pDevices = new RAWINPUTDEVICE[numDevices];

	GetRegisteredRawInputDevices(pDevices, &numDevices, sizeof(RAWINPUTDEVICE));

	delete[] pDevices;*/
}

void InputManager::SetDeviceChange(WPARAM wParam, LPARAM lParam)
{
	std::wstring deviceName;
	UINT unBuffSize = sizeof(RID_DEVICE_INFO);

	GetRawInputDeviceInfo((HANDLE)lParam, RIDI_DEVICENAME, nullptr, &unBuffSize);

	if(unBuffSize)
	{
		deviceName.resize(unBuffSize);
		if(GetRawInputDeviceInfo((HANDLE)lParam, RIDI_DEVICENAME, &deviceName[0], &unBuffSize) == (UINT)-1)
		{ 
			return;
		}
	}

	RID_DEVICE_INFO deviceInfo;
	unBuffSize = sizeof(RID_DEVICE_INFO);
	GetRawInputDeviceInfo((HANDLE)lParam, RIDI_DEVICEINFO, &deviceInfo, &unBuffSize);
}

void InputManager::ProcessInput(WPARAM wParam, LPARAM lParam)
{
#if USE_BUFFERED_INPUT
	UINT unSize = 0;

	// Get Buffer Size
	if(GetRawInputBuffer(NULL, &unSize, sizeof(RAWINPUTHEADER)) != 0 || !unSize)
	{
		return;
	}

	PRAWINPUT pInput = (PRAWINPUT)malloc(++unSize);

	unsigned int unBuffSize = GetRawInputBuffer(pInput, &unSize, sizeof(RAWINPUTHEADER));

	if(unBuffSize != (UINT)-1)
	{
		for (unsigned int i = 0; i < unBuffSize; ++i)
		{
			switch (pInput->header.dwType)
			{
				case RIM_TYPEMOUSE:
				{

				}
				break;

				case RIM_TYPEKEYBOARD:
				{
					PostKeyboardEvent(pInput);
				}
				break;
			}
			NEXTRAWINPUTBLOCK(pInput);
		}
	}
	else
	{
		/*std::wostringstream woss;
		woss << L"GetRawInputBuffer() Failed with Code(" << GetLastError() << L")";
		MessageBox(NULL, woss.str().c_str(), L"", MB_OK | MB_ICONEXCLAMATION);*/
	}

	free(pInput);
#else
	UINT unSize;

	GetRawInputData((HRAWINPUT)lParam, RID_INPUT, NULL, &unSize, sizeof(RAWINPUTHEADER));

	if (unSize)
	{
		char* pData = new char[unSize];

		if (GetRawInputData((HRAWINPUT)lParam, RID_INPUT, pData, &unSize, sizeof(RAWINPUTHEADER)) != (UINT)-1)
		{
			RAWINPUT* pInput = (RAWINPUT*)pData;

			switch (pInput->header.dwType)
			{
			case RIM_TYPEMOUSE:
				{
					PostMouseEvent(pInput);
				}
				break;

			case RIM_TYPEKEYBOARD:
				{
					CreateKeyboardEvent(pInput);
				}
				break;
			}
		}

		delete[] pData;
	}
#endif
}

void InputManager::CreateKeyboardEvent(PRAWINPUT pInput)
{
	RAWKEYBOARD& rwKeyboard = pInput->data.keyboard;

	InputButtonEvent newEvent;
	
	newEvent.eInputID = EInputID(rwKeyboard.MakeCode | ((rwKeyboard.Flags & RID_ALT_SCANCODE_FLAG) << RID_SCANCODE_OFFSET));
	newEvent.eInputState = (rwKeyboard.Flags & RID_KEY_UP_FLAG) ? IS_Released : IS_Pressed;
	newEvent.ucModifiers = m_ucMofifiers;
	newEvent.eDeviceID = DID_Keyboard;

	PostInputEvent(newEvent);

	cout << "Keyboard - Flags: " << rwKeyboard.Flags << '\t' << "Scan Code: " << rwKeyboard.MakeCode << "\tInput ID: " << newEvent.eInputID << endl;
}

void InputManager::PostMouseEvent(PRAWINPUT pInput)
{
	RAWMOUSE& rwMouse = pInput->data.mouse;

	// if this is a movement message because raw input apparently wont send
	// a movement message and a button message at the same time.
	if(!rwMouse.usFlags)
	{
		InputMouseMoveEvent newEvent;

		newEvent.vMouseMovement = Vec2f(rwMouse.lLastX, rwMouse.lLastY);

		PostInputEvent(newEvent);
	}
	else
	{
		InputMouseButtonEvent newEvent;
		/*newEvent.eDeviceID = DID_Mouse;
		newEvent.eInputState
		newEvent.ucModifiers = m_ucMofifiers;*/
	}

	cout << "Mouse - Button Flags: " << rwMouse.usButtonFlags << "\tusButtonData: " << rwMouse.usButtonData << "\tLast X: " << rwMouse.lLastX << "\tLast Y: " << rwMouse.lLastY << endl;
}

void InputManager::PostInputEvent(const IInputEvent& inputEvent)
{
	map<IInputEvent, IInputListener*>::iterator iter = m_mInputListeners.find(inputEvent);

	//// if we have a listener for the event
	//if(iter != m_mInputListeners.end())
	//{
	//	if(iter->second)
	//		iter->second->OnInputEvent(inputEvent);
	//	else
	//		m_mInputListeners.erase(iter);
	//}
}

void InputManager::RegisterEventListener(IInputListener* pListener, const IInputEvent& inputEvent, INPUT_EVENT_HANDLER fpEventHandler)
{
	
}