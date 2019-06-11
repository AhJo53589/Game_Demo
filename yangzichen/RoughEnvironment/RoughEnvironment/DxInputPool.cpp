#include "DxInputPool.h"

namespace pool
{
	CDxInputPool::CDxInputPool() : CDeviceErrorInterface(TEXT("DxInputPool")),
		mInput(NULL), mKeyboard(NULL), mMouse(NULL), mxMPos(0), myMPos(0), mxMPosDelta(0), myMPosDelta(0), mzMPosDelta(0)
	{
		memset(&(this->mMouseState), 0, sizeof(this->mMouseState));
		memset(&(this->moldMouseState), 0, sizeof(this->moldMouseState));

		memset(&(this->mKeys), 0, sizeof(this->mKeys));
		memset(&(this->moldKeys), 0, sizeof(this->moldKeys));
	}

	CDxInputPool::~CDxInputPool()
	{
		this->ClearUp();
	}

	bool CDxInputPool::Initialize(PGAME game, bool mouseExclusive)
	{
		this->ClearUp();

		if (FAILED(DirectInput8Create(game->GethInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&(this->mInput), NULL)))
		{
			this->ErrorMessage(TEXT("创建DirectInput8Create对象失败"), game->GethWnd());
			return false;
		}

		if (FAILED(this->mInput->CreateDevice(GUID_SysKeyboard, &(this->mKeyboard), NULL)))
		{
			this->ErrorMessage(TEXT("创建键盘对象失败"), game->GethWnd());
			return false;
		}

		if (FAILED(this->mKeyboard->SetDataFormat(&c_dfDIKeyboard)))
		{
			this->ErrorMessage(TEXT("键盘SetDataFormat失败"), game->GethWnd());
			return false;
		}

		//设置合作等级 DISCL_FOREGROUND 后台时不获取, DISCL_NONEXCLUSIVE 共享设备
		if (FAILED(this->mKeyboard->SetCooperativeLevel(game->GethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			this->ErrorMessage(TEXT("键盘设置合作等级失败"), game->GethWnd());
			return false;
		}

		//获取设备
		this->mKeyboard->Acquire();

		memset(this->mKeys, 0, sizeof(this->mKeys));


		DWORD flag;
		if (mouseExclusive)
			flag = DISCL_EXCLUSIVE | DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
		else
			flag = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;

		if (FAILED(this->mInput->CreateDevice(GUID_SysMouse, &(this->mMouse), NULL)))
		{
			this->ErrorMessage(TEXT("创建鼠标对象失败"), game->GethWnd());
			return false;
		}

		if (FAILED(this->mMouse->SetDataFormat(&c_dfDIMouse)))
		{
			this->ErrorMessage(TEXT("鼠标SetDataFormat失败"), game->GethWnd());
			return false;
		}

		if (FAILED(this->mMouse->SetCooperativeLevel(game->GethWnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
		{
			this->ErrorMessage(TEXT("鼠标设置合作等级失败"), game->GethWnd());
			return false;
		}

		this->mMouse->Acquire();

		return true;
	}

	bool CDxInputPool::UpdateDevice()
	{
		if (this->mMouse)
		{
			memcpy(&(this->moldMouseState), &(this->mMouseState), sizeof(this->mMouseState));

			if (FAILED(this->mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &(this->mMouseState))))
			{
				if (FAILED(mMouse->Acquire()))
				{
					return false;
				}

				if (FAILED(this->mMouse->GetDeviceState(sizeof(DIMOUSESTATE), &(this->mMouseState))))
				{
					return false;
				}
			}

			this->mxMPos += this->moldMouseState.lX;
			this->myMPos += this->moldMouseState.lY;
			this->mxMPosDelta = this->moldMouseState.lX;
			this->myMPosDelta = this->moldMouseState.lY;
			this->mzMPosDelta = this->moldMouseState.lZ;
		}

		if (this->mKeyboard)
		{
			memcpy(&(this->moldKeys), &(this->mKeys), sizeof(this->mKeys));

			if (FAILED(this->mKeyboard->GetDeviceState(sizeof(this->mKeys), (LPVOID)(this->mKeys))))
			{
				if (FAILED(this->mKeyboard->Acquire()))
				{
					return false;
				}

				if (FAILED(this->mKeyboard->GetDeviceState(sizeof(this->mKeys), (LPVOID)(this->mKeys))))
				{
					return false;
				}
			}
		}

		return true;
	}

	int CDxInputPool::KeyUp(unsigned long key)
	{
		return (!(this->mKeys[key] & 0x80) && this->mKeys[key] !=moldKeys[key]);
	}

	int CDxInputPool::KeyDown(unsigned long key)
	{
		return (this->mKeys[key] & 0x80);
	}

	int CDxInputPool::MouseButtonUp(unsigned long button)
	{
		return (!(this->mMouseState.rgbButtons[button] & 0x80) &&
			this->mMouseState.rgbButtons[button] != this->moldMouseState.rgbButtons[button]);
	}

	int CDxInputPool::MouseButtonDown(unsigned long button)
	{
		return (this->mMouseState.rgbButtons[button] & 0x80);
	}

	POINT CDxInputPool::GetMousePos()
	{
		POINT p;
		p.x = this->mxMPos;
		p.y = this->myMPos;
		return p;
	}

	POINT CDxInputPool::GetMousePosDelta()
	{
		POINT p;
		p.x = this->mxMPosDelta;
		p.y = this->myMPosDelta;
		return p;
	}

	long CDxInputPool::GetMouseWheelPosDelta()
	{
		return this->mzMPosDelta;
	}

	void CDxInputPool::ClearUp()
	{
		if (this->mKeyboard)
		{
			this->mKeyboard->Unacquire();
			this->mKeyboard->Release();
			this->mKeyboard = NULL;
		}

		if (this->mMouse)
		{
			this->mMouse->Unacquire();
			this->mMouse->Release();
			this->mMouse = NULL;
		}

		if (this->mInput)
		{
			this->mInput->Release();
			this->mInput = NULL;
		}
	}
}
