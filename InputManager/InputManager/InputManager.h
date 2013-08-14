#ifndef	INPUTMANAGER_H
#define INPUTMANAGER_H

typedef void (*InputMessageProc)();

class InputManager
{
private:


public:
	InputManager(void);
	~InputManager(void);

	bool Initialize();

	void ProcessInput();
	void ClearInput();
};

#endif // INPUTMANAGER_H