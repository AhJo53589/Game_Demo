#pragma once

#define KEYS_SIZE 256
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#include "define.h"
#include "GameInterface.h"

namespace pool
{
	namespace Mouse
	{
		const unsigned long LEFT_BUTTON = 0;
		const unsigned long RIGHT_BUTTON = 1;
	}
	
	class CInputInterface
	{
	public:
		CInputInterface()
		{
		}
		~CInputInterface()
		{
		}

		virtual bool Initialize(PGAME game, bool mouseExclusive = false) = 0;
		virtual bool UpdateDevice() = 0;

		virtual int KeyUp(unsigned long key) = 0;
		virtual int KeyDown(unsigned long key) = 0;

		virtual int MouseButtonUp(unsigned long button) = 0;
		virtual int MouseButtonDown(unsigned long button) = 0;

		virtual POINT GetMousePos() = 0;
		virtual POINT GetMousePosDelta() = 0;
		virtual long GetMouseWheelPosDelta() = 0;

	};

	typedef CInputInterface *PINPUT;
}