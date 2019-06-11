#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include "Util.h"

class DInput;

class InputEventListener
{
public:
	virtual void InputEvent(DInput *n, float dt) = 0;
};

class Game2drpg;

class DInput
{
public:
	DInput(Game2drpg *gamebase);
	~DInput();

	void updateDevice(float dt);

	bool keyDown(unsigned long key);

	bool mouseButtonDown(unsigned long button);

	float mouseDX();
	float mouseDY();
	float mouseDZ();

	void addListener(InputEventListener *l);
    void removeListener(InputEventListener *l);

private:
	IDirectInput8*       mDxInput;

	IDirectInputDevice8* mKeyboard;
	char                 mKeyboardState[256]; 

	IDirectInputDevice8* mMouse;
	DIMOUSESTATE2        mMouseState;

	std::vector<InputEventListener *> mListenerList;

	void notifyInputEvent(float dt);
};
