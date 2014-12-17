#ifndef INPUT_H
#define INPUT_H

#include "../Utilities/UtilitiesInclude.h"
#include "../Math Lib/MathLib.h"
#include "../Utilities/FunctionPointer.h"

#ifdef _WIN64
typedef long long QWORD;
#endif

enum EInputState : unsigned char
{
	IS_Pressed	= 0x0,
	IS_Down		= 0x1,
	IS_Released	= 0x2,
	IS_Changed	= 0x4
};

enum EDeviceID : unsigned char
{
	DID_None		= 0x0,
	DID_Mouse		= 0x1,
	DID_Keyboard	= 0x2,
	DID_Gamepad1	= 0x4,
	DID_Gamepad2	= 0x8,
	DID_Gamepad3	= 0x10,
	DID_Gamepad4	= 0x20,

	DID_GamepadAny	= (DID_Gamepad1 | DID_Gamepad2 | DID_Gamepad3 | DID_Gamepad4)
};

enum EInputModifier : unsigned char
{
	IM_None		= 0,
	IM_LCtrl	= 0x1,
	IM_LShift	= 0x2,
	IM_LAlt		= 0x4,
	IM_RCtrl	= 0x8,
	IM_RShift	= 0x10,
	IM_RAlt		= 0x20,

	IM_Ctrl		= (IM_LCtrl | IM_RCtrl),
	IM_Shift	= (IM_LShift | IM_RShift),
	IM_Alt		= (IM_LAlt | IM_RAlt)
};

enum EInputID : unsigned short
{
	// Keyboard
	KID_None	= 0,
	KID_Esc		= 1,
	KID_1,
	KID_2,
	KID_3,
	KID_4,
	KID_5,
	KID_6,
	KID_7,
	KID_8,
	KID_9,
	KID_0,
	KID_Minus,
	KID_Equals,
	KID_Backspace,
	KID_Tab,
	KID_Q,
	KID_W,
	KID_E,
	KID_R,
	KID_T,
	KID_Y,
	KID_U,
	KID_I,
	KID_O,
	KID_P,
	KID_LBracket,
	KID_RBracket,
	KID_Enter,
	KID_LCtrl,
	KID_A,
	KID_S,
	KID_D,
	KID_F,
	KID_G,
	KID_H,
	KID_J,
	KID_K,
	KID_L,
	KID_Semicolon,
	KID_Apostrophe,
	KID_Tilde,
	KID_LShift,
	KID_Backslash,
	KID_Z,
	KID_X,
	KID_C,
	KID_V,
	KID_B,
	KID_N,
	KID_M,
	KID_Comma,
	KID_Period,
	KID_Slash,
	KID_RShift,
	KID_NUM_Asterisk,
	KID_LAlt,
	KID_Space,
	KID_CapsLock,
	KID_F1,
	KID_F2,
	KID_F3,
	KID_F4,
	KID_F5,
	KID_F6,
	KID_F7,
	KID_F8,
	KID_F9,
	KID_F10,
	KID_NumLock,
	KID_ScrollLock,
	KID_NUM_7,
	KID_NUM_8,
	KID_NUM_9,
	KID_NUM_Minus,
	KID_NUM_4,
	KID_NUM_5,
	KID_NUM_6,
	KID_NUM_Plus,
	KID_NUM_1,
	KID_NUM_2,
	KID_NUM_3,
	KID_NUM_0,
	KID_NUM_Period,
	KID_NUM_Divide	= (53 | 0x80),			// 53 + (flags & (0x2))
	KID_NUM_Enter	= (28 | 0x80),			// 28 + (flags & (0x2))
	// 86 is skipped for some reason...
	KID_F11			= 87,					// 87
	KID_F12,								// 88
	KID_Insert		= (82 | (0x80)),		// 82 + (flags & (0x2))
	KID_Delete		= (83 | (0x80)),		// 83 + (flags & (0x2))
	KID_Home		= (71 | (0x80)),		// 71 + (flags & (0x2))
	KID_End			= (79 | (0x80)),		// 79 + (flags & (0x2))
	KID_PageUp		= (73 | (0x80)),		// 73 + (flags & (0x2))
	KID_PageDown	= (81 | (0x80)),		// 81 + (flags & (0x2))
	KID_Left		= (75 | (0x80)),		// 75 + (flags & (0x2))
	KID_Right		= (77 | (0x80)),		// 77 + (flags & (0x2))
	KID_Up			= (72 | (0x80)),		// 72 + (flags & (0x2))
	KID_Down		= (80 | (0x80)),		// 80 + (flags & (0x2))
	KID_RAlt		= (56 | (0x80)),		// 56 + (flags & (0x2))
	KID_RCtrl		= (29 | (0x80)),		// 29 + (flags & (0x2))

	// Mouse
	KID_MouseButton1 = 0x100,
	KID_MouseLButton = KID_MouseButton1,
	KID_MouseButton2,
	KID_MouseRButton = KID_MouseButton2,
	KID_MouseButton3,
	KID_MouseMButton = KID_MouseButton3,
	KID_MouseButton4,
	KID_MouseButton5,
	KID_MouseWheelUp,
	KID_MouseWheelDown,
	KID_MouseMove
};

struct InputEvent
{
	EInputID m_eInputID			: 16;
	EInputState m_eInputState	: 4;
	u32 m_eModifiers			: 6;
	EDeviceID m_eDeviceID		: 6;

	InputEvent() : m_eInputID(KID_None), m_eDeviceID(DID_None), m_eInputState(IS_Changed), m_eModifiers(IM_None) {}
	InputEvent(EDeviceID eDID) : m_eDeviceID(eDID) {}
	InputEvent(EDeviceID eDID, EInputID eKID, EInputState eIS, u8 eModifiers = IM_None)
			: m_eDeviceID(eDID), m_eInputID(eKID), m_eInputState(eIS), m_eModifiers(eModifiers) {}

	virtual ~InputEvent() {}

	bool operator==(const InputEvent& rhs) const
	{
		if(m_eInputID != rhs.m_eInputID)
			return false;
		if(m_eInputState < rhs.m_eInputState)
			return false;
		if(m_eModifiers < rhs.m_eModifiers)
			return false;
		if(m_eDeviceID < rhs.m_eDeviceID)
			return false;	

		return true;
	}

	bool operator<(const InputEvent& rhs) const
	{
		if((m_eInputID < rhs.m_eInputID) ||
		   (m_eInputState < rhs.m_eInputState) ||
		   (m_eModifiers < rhs.m_eModifiers) ||
		   (m_eDeviceID < rhs.m_eDeviceID))
		{
			return true;
		}

		return false;
	}

	bool IsSameButton(const InputEvent& rhs) const
	{
		if(m_eInputID != rhs.m_eInputID)
			return false;
		if(m_eInputState != rhs.m_eInputState)
			return false;
		if(m_eDeviceID != rhs.m_eDeviceID)
			return false;

		return true;
	}

	virtual InputEvent* MakeNewCopy() const
	{
		return new InputEvent(*this);
	}
};

struct InputMouseButtonEvent : public InputEvent
{
	Vec2i m_vMousePos;

	InputMouseButtonEvent() : InputEvent(DID_Mouse, KID_None, IS_Down) {}
	InputMouseButtonEvent(EInputID eKID, EInputState eIS, EInputModifier modifiers, Vec2i vCurrMousePos)
		: InputEvent(DID_Mouse, eKID, eIS, modifiers), m_vMousePos(vCurrMousePos) {}

	virtual ~InputMouseButtonEvent() {}

	virtual InputEvent* MakeNewCopy() const
	{
		return new InputMouseButtonEvent(*this);
	}
};

struct InputMouseMoveEvent : public InputEvent
{
	Vec2i m_vMousePos;
	Vec2f m_vMouseMovement;

	InputMouseMoveEvent() : InputEvent(DID_Mouse, KID_MouseMove, IS_Changed) {}
	InputMouseMoveEvent(Vec2i vCurrMousePos, Vec2f vCurrMouseMovement)
		: InputEvent(DID_Mouse, KID_MouseMove, IS_Changed, IM_None), m_vMousePos(vCurrMousePos), m_vMouseMovement(vCurrMouseMovement) {}

	virtual ~InputMouseMoveEvent() {}

	virtual InputEvent* MakeNewCopy() const
	{
		return new InputMouseMoveEvent(*this);
	}
};

typedef FunctionPointer<void, const InputEvent&> InputCallbackfn;

#endif