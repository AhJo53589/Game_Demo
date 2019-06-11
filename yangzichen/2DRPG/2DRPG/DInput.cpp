#include "DInput.h"
#include "Game2drpg.h"

DInput::DInput(Game2drpg *gamebase)
{
	ZeroMemory(this->mKeyboardState, sizeof(mKeyboardState));
	ZeroMemory(&this->mMouseState, sizeof(mMouseState));

	HR(DirectInput8Create(gamebase->getInst(), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&this->mDxInput, 0));

	HR(mDxInput->CreateDevice(GUID_SysKeyboard, &this->mKeyboard, 0));
	HR(mKeyboard->SetDataFormat(&c_dfDIKeyboard));
	//设置合作等级 DISCL_FOREGROUND 后台时不获取, DISCL_NONEXCLUSIVE 共享设备
	HR(mKeyboard->SetCooperativeLevel(gamebase->getWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	mKeyboard->Acquire();

	HR(mDxInput->CreateDevice(GUID_SysMouse, &this->mMouse, 0));
	HR(mMouse->SetDataFormat(&c_dfDIMouse2));
	HR(mMouse->SetCooperativeLevel(gamebase->getWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	mMouse->Acquire();
}


DInput::~DInput(void)
{
	ReleaseCOM(this->mDxInput);
	this->mKeyboard->Unacquire();
	this->mMouse->Unacquire();
	ReleaseCOM(this->mKeyboard);
	ReleaseCOM(this->mMouse);
}

void DInput::updateDevice(float dt)
{
	HRESULT hr = this->mKeyboard->GetDeviceState(sizeof(this->mKeyboardState), (void**)&this->mKeyboardState); 
	if( FAILED(hr) )
	{
		ZeroMemory(this->mKeyboardState, sizeof(this->mKeyboardState));
		hr = this->mKeyboard->Acquire();
	}

	hr = mMouse->GetDeviceState(sizeof(DIMOUSESTATE2), (void**)&this->mMouseState); 
	if( FAILED(hr) )
	{
		ZeroMemory(&this->mMouseState, sizeof(this->mMouseState));
		hr = this->mMouse->Acquire(); 
	}

	this->notifyInputEvent(dt);
}

bool DInput::keyDown(unsigned long key)
{
	return (this->mKeyboardState[key] & 0x80) != 0;
}

bool DInput::mouseButtonDown(unsigned long button)
{
	return (this->mMouseState.rgbButtons[button] & 0x80) != 0;
}

float DInput::mouseDX()
{
	return (float)this->mMouseState.lX;
}

float DInput::mouseDY()
{
	return (float)this->mMouseState.lY;
}

float DInput::mouseDZ()
{
	return (float)this->mMouseState.lZ;
}

void DInput::addListener(InputEventListener *l)
{
	if (l)
	{
		this->mListenerList.push_back(l);
	}
}

void DInput::removeListener(InputEventListener *l)
{
	for(std::vector<InputEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		if(*it == l)
		{
			this->mListenerList.erase(it);
			return;
		}
	}
}

void DInput::notifyInputEvent(float dt)
{
	for(std::vector<InputEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		(*it)->InputEvent(this, dt);
	}
}