#include "InputManager.h"
#include "../Utilities/UtilitiesInclude.h"
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

#define MOUSE_BUTTON_MASK(buttonIndex) ((0x3) << ((buttonIndex)<<1))
#define MOUSE_BUTTON_DOWN_MASK (0x155)
#define MOUSE_BUTTON_UP_MASK (0x2AA)

#define USE_BUFFERED_INPUT (0)

InputManager g_InputManager;

InputManager::InputManager() : 
m_ucModifiers(IM_None), 
m_ucConnectedDevices(DID_Mouse | DID_Keyboard), 
m_WindowHandle(nullptr)
{

}

InputManager::~InputManager()
{
	Shutdown();
}

void InputManager::Shutdown()
{
	m_PendingInputCS.Lock();
	while(!m_PendingInputEvents.empty())
	{
		m_PendingInputEvents.pop();
	}
	m_NewInputEvent.Signal();
	m_PendingInputCS.Unlock();

	ClearAllCurrentInput();

	m_InputHook.Shutdown();
	m_InputThread.EndThread();

	SysCloseEvent(m_NewInputEvent);
}

bool InputManager::Initialize(HWND hWnd)
{
	m_WindowHandle = hWnd;

	RAWINPUTDEVICE Rid[2];
	ZeroMemory(Rid, sizeof(Rid));

	Rid[0].usUsagePage = RID_USAGE_PAGE_GENERIC;
	Rid[0].usUsage = RID_USAGE_MOUSE;
	Rid[0].dwFlags = RIDEV_DEVNOTIFY;
	Rid[0].hwndTarget = (HWND)m_WindowHandle;

	Rid[1].usUsagePage = RID_USAGE_PAGE_GENERIC;
	Rid[1].usUsage = RID_USAGE_KEYBOARD;
	Rid[1].dwFlags = RIDEV_DEVNOTIFY;
	Rid[1].hwndTarget = (HWND)m_WindowHandle;

	if (FALSE == RegisterRawInputDevices(Rid, sizeof(Rid) / sizeof(*Rid), sizeof(RAWINPUTDEVICE)))
	{
		MessageBox(NULL, _T("Failed To Register Raw Input Devices"), _T(""), MB_OK | MB_ICONERROR);
		return false;
	}

	m_NewInputEvent = SysCreateEvent();

	m_InputThread.StartThread(CreateFunctionPointer(this, &InputManager::InputThreadProc), nullptr, THREAD_PRIO_NORMAL, "Input Thread");
	m_InputHook.Init(HOOK_GETMESSAGE, InputManager::InputHookCallback);

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

void InputManager::OnWindowFocusActivate()
{

}

void InputManager::OnWindowFocusDeactivate()
{
	ClearAllCurrentInput();
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

void InputManager::ProcessInputEvent(WPARAM wParam, LPARAM lParam)
{
	//LocalCriticalSection inputCS(m_InputCS);

#if USE_BUFFERED_INPUT
	UINT unSize = 0;

	// Get Buffer Size
	if(GetRawInputBuffer(NULL, &unSize, sizeof(RAWINPUTHEADER)) != 0 || !unSize)
	{
		return;
	}

	PRAWINPUT pInput = (PRAWINPUT)_aligned_malloc(unSize, 8);

	unsigned int unBuffSize = GetRawInputBuffer(pInput, &unSize, sizeof(RAWINPUTHEADER));

	if(unBuffSize != (UINT)-1)
	{
		for (unsigned int i = 0; i < unBuffSize; ++i)
		{
			switch (pInput->header.dwType)
			{
			case RIM_TYPEMOUSE:
				{
					CreateMouseEvent(pInput);
				}
				break;

			case RIM_TYPEKEYBOARD:
				{
					CreateKeyboardEvent(pInput);
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

	_aligned_free(pInput);
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
					CreateMouseEvent(pInput);
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

void InputManager::AddInputRequest(WPARAM wParam, LPARAM lParam)
{
	PendingInputEvent pendingEvent = {wParam, lParam};

	m_PendingInputCS.Lock();
	m_PendingInputEvents.push(pendingEvent);
	m_PendingInputCS.Unlock();

	m_NewInputEvent.Signal();
}

void InputManager::ProcessInput()
{
#if DEBUG
	DisplayCurrentEvents();
#endif // DEBUG

	LocalCriticalSection currInputCS(m_CurrentInputCS);

	list<InputCurrentEventContainer::iterator> lEventsToDelete;

	for(InputCurrentEventContainer::iterator inputEventIter = m_lCurrentEvents.begin(); inputEventIter != m_lCurrentEvents.end(); ++inputEventIter)
	{
		InputEvent*& pInputEvent = *inputEventIter;
		if(pInputEvent)
		{
			InputCallbackContainer::iterator inputCallbackIter = m_mInputCallbacks.find(*pInputEvent);

			if(inputCallbackIter != m_mInputCallbacks.end())
			{
				inputCallbackIter->second(*pInputEvent);
			}

			switch(pInputEvent->m_eInputState)
			{
				case IS_Pressed:
				{
					pInputEvent->m_eInputState = IS_Down;
				}
				break;

				case IS_Released:
				case IS_Changed:
				{
					delete pInputEvent;
					pInputEvent = nullptr;

					lEventsToDelete.push_back(inputEventIter);
				}
				break;
			}
		}
		else
		{
			lEventsToDelete.push_back(inputEventIter);
		}
	}

	for(list<InputCurrentEventContainer::iterator>::iterator iter = lEventsToDelete.begin(); iter != lEventsToDelete.end(); ++iter)
	{
		m_lCurrentEvents.erase(*iter);
	}
}

InputEvent* InputManager::GetCurrentInputEvent(const InputEvent& inputEvent) const
{
	InputEvent* pInputEvent = nullptr;

	for(InputCurrentEventContainer::const_iterator iter = m_lCurrentEvents.cbegin(); iter != m_lCurrentEvents.cend(); ++iter)
	{
		InputEvent* const pCurrEvent = (*iter);

		if(inputEvent.IsSameButton(*pCurrEvent))
		{
			pInputEvent = pCurrEvent;
			break;
		}
	}

	return pInputEvent;
}

void InputManager::HandleModifierInput(const InputEvent& inputEvent)
{
	switch(inputEvent.m_eInputID)
	{
	case KID_LShift:
		{
			SetInputModifier(IM_LShift, inputEvent.m_eInputState < IS_Released);
		}
		break;

	case KID_RShift:
		{
			SetInputModifier(IM_RShift, inputEvent.m_eInputState < IS_Released);
		}
		break;

	case KID_LCtrl:
		{
			SetInputModifier(IM_LCtrl, inputEvent.m_eInputState < IS_Released);
		}
		break;

	case KID_RCtrl:
		{
			SetInputModifier(IM_RCtrl, inputEvent.m_eInputState < IS_Released);
		}
		break;

	case KID_LAlt:
		{
			SetInputModifier(IM_LAlt, inputEvent.m_eInputState < IS_Released);
		}
		break;

	case KID_RAlt:
		{
			SetInputModifier(IM_RAlt, inputEvent.m_eInputState < IS_Released);
		}
		break;
	}
}

void InputManager::SetInputModifier(EInputModifier eModifier, bool bSet)
{
	if(Verify(eModifier > IM_None && eModifier <= IM_RAlt, "Input Manager - Trying To Set An Invalid Modifier"))
	{
		m_ucModifiers = bSet ? (m_ucModifiers | eModifier) : (m_ucModifiers & ~eModifier);
	}
}

void InputManager::CreateKeyboardEvent(PRAWINPUT pInput)
{
	RAWKEYBOARD& rwKeyboard = pInput->data.keyboard;

	InputEvent newEvent;
	
	newEvent.m_eInputID = EInputID(rwKeyboard.MakeCode | ((rwKeyboard.Flags & RID_ALT_SCANCODE_FLAG) << RID_SCANCODE_OFFSET));
	newEvent.m_eInputState =  IS_Down;
	newEvent.m_eModifiers = m_ucModifiers;
	newEvent.m_eDeviceID = DID_Keyboard;

	PostButtonEvent(newEvent, !(rwKeyboard.Flags & RID_KEY_UP_FLAG));

	HandleModifierInput(newEvent);
}

void InputManager::CreateMouseEvent(PRAWINPUT pInput)
{
	RAWMOUSE& rwMouse = pInput->data.mouse;

	// if this is a movement message because raw input apparently wont send
	// a movement message and a button message at the same time.
	if(!rwMouse.usButtonFlags)
	{
		InputMouseMoveEvent newEvent;

		newEvent.m_vMouseMovement = Vec2f((float)rwMouse.lLastX, (float)rwMouse.lLastY);
		newEvent.m_vMousePos = GetMousePosition();

		PostInputEvent(&newEvent);
	}
	else
	{
		InputMouseButtonEvent newEvent;
		PostMouseButtonEvent(rwMouse, newEvent);
	}

	//cout << "Mouse - Button Flags: " << rwMouse.usButtonFlags << "\tusButtonData: " << (SHORT)rwMouse.usButtonData << "\tRaw Buttons: " << rwMouse.ulRawButtons <<"\n\tLast X: " << rwMouse.lLastX << "\tLast Y: " << rwMouse.lLastY << endl;
}

void InputManager::PostButtonEvent(InputEvent& inputEvent, bool bIsDown)
{
	LocalCriticalSection currInputCS(m_CurrentInputCS);

	InputEvent* pDownEvent = GetCurrentInputEvent(inputEvent);

	if(bIsDown) // Key Down
	{
		if(!pDownEvent)
		{
			inputEvent.m_eInputState = IS_Pressed;
			PostInputEvent(&inputEvent);
		}
	}
	else
	{
		inputEvent.m_eInputState = IS_Released;

		if(pDownEvent)
		{
			pDownEvent->m_eInputState = IS_Released;
			PostInputEvent(pDownEvent);
		}
		else
		{			
			PostInputEvent(&inputEvent);
		}
	}
}

void InputManager::PostInputEvent(const InputEvent* pInputEvent)
{
	if(pInputEvent)
	{
		LocalCriticalSection currInputCS(m_CurrentInputCS);

		InputEvent* pCurrEvent = GetCurrentInputEvent(*pInputEvent);

		if(!pCurrEvent)
		{
			m_lCurrentEvents.push_back(pInputEvent->MakeNewCopy());
		}

		if(pInputEvent->m_eInputState != IS_Changed)
		{
			InputEvent* pChangedEvent = pInputEvent->MakeNewCopy();

			pChangedEvent->m_eInputState = IS_Changed;
			m_lCurrentEvents.push_back(pChangedEvent);
		}
	}
}

void InputManager::ClearAllCurrentInput()
{
	LocalCriticalSection currInputCS(m_CurrentInputCS);

	for(InputCurrentEventContainer::iterator iter = m_lCurrentEvents.begin(); iter != m_lCurrentEvents.end(); ++iter)
	{
		SAFE_DELETE(*iter);
	}

	m_lCurrentEvents.clear();

	cout << "Input Cleared!";
}

void InputManager::InputThreadProc(void* pArgs)
{
	volatile bool bRunning = true;
	while(bRunning)
	{
		m_NewInputEvent.Wait();

		m_PendingInputCS.Lock();

		if(m_PendingInputEvents.size() > 0)
		{
			PendingInputEvent inputEvent = m_PendingInputEvents.front();
			m_PendingInputEvents.pop();
			m_PendingInputCS.Unlock();

			ProcessInputEvent(inputEvent.wParam, inputEvent.lParam);
			cout << "Event Posted!\n";
		}
		else
		{
			m_PendingInputCS.Unlock();
			bRunning = false;
		}
	}
}

void InputManager::PostMouseButtonEvent(RAWMOUSE& rwMouse, InputMouseButtonEvent& newEvent)
{
	newEvent.m_eModifiers = m_ucModifiers;
	newEvent.m_vMousePos = GetMousePosition();

	if(rwMouse.usButtonFlags != RI_MOUSE_WHEEL)
	{
		for(int i = 0; i < 5; ++i)
		{
			if((rwMouse.usButtonFlags & MOUSE_BUTTON_MASK(i)))
			{
				newEvent.m_eInputID = EInputID(KID_MouseButton1 + i);
				break;
			}
		}

		PostButtonEvent(newEvent, ((rwMouse.usButtonFlags & MOUSE_BUTTON_DOWN_MASK) != 0));
	}
	else
	{
		// If The High Bit Is Set Then That Means the Wheel Was Scrolled Down
		newEvent.m_eInputID = (rwMouse.usButtonData & 0x8000) ? KID_MouseWheelDown : KID_MouseWheelUp;
		newEvent.m_eInputState = IS_Changed;

		PostInputEvent(&newEvent);
	}
}

void InputManager::RegisterEventCallback(InputEvent& inputEvent, InputCallbackfn& fpEventHandler, bool bOverrideCallback /*= false*/)
{
	InputCallbackContainer::iterator iter = m_mInputCallbacks.find(inputEvent);

	if(iter != m_mInputCallbacks.end())
	{
		m_mInputCallbacks.insert(std::make_pair(inputEvent, fpEventHandler));
	}
}

Vec2i InputManager::GetMousePosition()
{
	POINT cursorPos;
	GetCursorPos(&cursorPos);
	return Vec2i(cursorPos.x, cursorPos.y);
}

LRESULT __stdcall InputManager::InputHookCallback(int code, WPARAM wParam, LPARAM lParam)
{
	if(code > -1)
	{
		MSG* pMSG = (MSG*)lParam;

		//if(pMSG->hwnd == g_InputManager.m_WindowHandle)
		{
			switch(pMSG->message)
			{
			case WM_INPUT:
				{
					g_InputManager.AddInputRequest(pMSG->wParam, pMSG->lParam);

					// For Some Reason if we don't Sleep then the messages get delayed and can hang for many frames...
					// It acts as though the window message queue is getting flooded which doesn't make sense since
					// each frame performs a while(PeekMessage(...)), which should be clearing the message queue!
					//Sleep(1);
				}
				break;

			case WM_INPUT_DEVICE_CHANGE:
				{
					//g_InputManager.SetDeviceChange(pMSG->wParam, pMSG->lParam);
				}
				break;

			case WM_ACTIVATE:
				{
					cout << "WM_ACTIVATE\n";
					if(pMSG->wParam != WA_INACTIVE)
					{
						g_InputManager.OnWindowFocusActivate();
						cout << "ACTIVE\n";
					}
					else
					{
						g_InputManager.OnWindowFocusDeactivate();
						cout << "INACTIVE\n";
					}
				}
				break;
			}
		}

		//code = -1;
	}

	return CallNextHookEx(nullptr, code, wParam, lParam);
}

#if DEBUG
void InputManager::DisplayCurrentEvents()
{
	//system("cls");

	LocalCriticalSection currInputCS(m_CurrentInputCS);

	for(InputCurrentEventContainer::iterator iter = m_lCurrentEvents.begin(); iter != m_lCurrentEvents.end(); ++iter)
	{
		InputEvent*& pInputEvent = (*iter);

		cout << "Input Event - ID: " << pInputEvent->m_eInputID << "  State: " << pInputEvent->m_eInputState << "  Modifiers: " << pInputEvent->m_eModifiers << "  Device: " << pInputEvent->m_eDeviceID << endl;
	}
}
#endif