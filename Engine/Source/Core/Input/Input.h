#ifndef INPUT_H
#define INPUT_H

#ifdef _WIN64
typedef long long QWORD;
#endif

enum EInputState
{
	IS_Pressed = 0x0,
	IS_Released = 0x1,
	IS_Down = 0x2,
	IS_Changed = 0x4
};

enum EDeviceID : unsigned char
{
	DID_Mouse = 0x1,
	DID_Keyboard = 0x2,
	DID_Gamepad1 = 0x4,
	DID_Gamepad2 = 0x8,
	DID_Gamepad3 = 0x10,
	DID_Gamepad4 = 0x20,

	DID_GamepadAny = (DID_Gamepad1 | DID_Gamepad2 | DID_Gamepad3 | DID_Gamepad4)
};

enum EInputModifiers : unsigned char
{
	IM_None = 0,
	IM_LCtrl = 0x1,
	IM_LShift = 0x2,
	IM_LAlt = 0x4,
	IM_RCtrl = 0x8,
	IM_RShift = 0x10,
	IM_RAlt = 0x20,

	IM_Ctrl = (IM_LCtrl | IM_RCtrl),
	IM_Shift = (IM_LShift | IM_RShift),
	IM_Alt = (IM_LAlt | IM_RAlt),
};

enum EInputID
{
	// Keyboard
	KID_Esc = 1,
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
	KID_NUM_Divide = (53 | 0x8), // 53 + (flags & (0x2))
	KID_NUM_Enter = (28 | 0x8),	// 28 + (flags & (0x2))
	// 86 is skipped for some reason...
	KID_F11 = 87, // 87
	KID_F12, // 88
	KID_Insert = (82 | (0x8)), // 82 + (flags & (0x2))
	KID_Delete = (83 | (0x8)), // 83 + (flags & (0x2))
	KID_Home = (71 | (0x8)), // 71 + (flags & (0x2))
	KID_End = (79 | (0x8)), // 79 + (flags & (0x2))
	KID_PageUp = (73 | (0x8)), // 73 + (flags & (0x2))
	KID_PageDown = (81 | (0x8)), // 81 + (flags & (0x2))
	KID_Left = (75 | (0x8)), // 75 + (flags & (0x2))
	KID_Right = (77 | (0x8)), // 77 + (flags & (0x2))
	KID_Up = (72 | (0x8)), // 72 + (flags & (0x2))
	KID_Down = (80 | (0x8)), // 80 + (flags & (0x2))
	KID_RAlt = (56 | (0x8)), // 56 + (flags & (0x2))
	KID_RCtrl = (29 | (0x8)), // 29 + (flags & (0x2))

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

struct IInputEvent
{
	EDeviceID eDeviceID;
	EInputID eInputID;
	EInputState eInputState;

	IInputEvent() {}
	IInputEvent(EDeviceID eDID, EInputID eKID, EInputState eIS)
			: eDeviceID(eDID), eInputID(eKID), eInputState(eIS) {}

	virtual ~IInputEvent() = 0 {}

	virtual bool operator<(const IInputEvent& rhs)
	{
		if((eDeviceID < rhs.eDeviceID) &&
		   (eInputID < rhs.eInputID) &&
		   (eInputState < rhs.eInputState))
		{
			return true;
		}

		return false;
	}
};

struct InputButtonEvent : public IInputEvent
{
	unsigned char ucModifiers;

	InputButtonEvent() {}
	InputButtonEvent(EDeviceID eDID, EInputID eKID, EInputState eIS, unsigned char modifiers) 
			: IInputEvent(eDID, eKID, eIS), ucModifiers(modifiers) {}

	virtual ~InputButtonEvent() {}

	virtual bool operator<(const IInputEvent& rhs)
	{
		if(!IInputEvent::operator<(rhs))
			return false;

		//if(rhs.de)
	}
};

struct InputMouseButtonEvent : public InputButtonEvent
{
	Vec2f vMousePos;

	InputMouseButtonEvent() {}
	InputMouseButtonEvent(EInputID eKID, EInputState eIS, unsigned char modifiers, Vec2f vCurrMousePos)
		: InputButtonEvent(DID_Mouse, eKID, eIS, modifiers), vMousePos(vCurrMousePos) {}

	virtual ~InputMouseButtonEvent() {}
};

struct InputMouseMoveEvent : public IInputEvent
{
	Vec2f vMousePos;
	Vec2f vMouseMovement;

	InputMouseMoveEvent() : IInputEvent(DID_Mouse, KID_MouseMove, IS_Changed) {}
	InputMouseMoveEvent(Vec2f vCurrMousePos, Vec2f vCurrMouseMovement)
		: IInputEvent(DID_Mouse, KID_MouseMove, IS_Changed), vMousePos(vCurrMousePos), vMouseMovement(vCurrMouseMovement) {}

	virtual ~InputMouseMoveEvent() {}
};

class IInputListener
{
private:

public:
	virtual ~IInputListener() = 0 {}

	virtual void OnInputEvent(const IInputEvent& pInputEvent) = 0;
};

struct InputMapComparison
{
	bool operator()(IInputEvent* lhs, IInputEvent* rhs)
	{
		return *lhs < *rhs;
	}
};

typedef void(IInputListener::*INPUT_EVENT_HANDLER)(const IInputEvent& inputEvent);

#endif