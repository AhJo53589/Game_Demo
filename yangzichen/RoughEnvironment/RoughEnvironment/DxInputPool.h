#pragma once

#include "InputInterface.h"
#include "ErrorInterface.h"

namespace pool
{
	class CDxInputPool : public CInputInterface, public CDeviceErrorInterface
	{
	public:
		CDxInputPool();
		~CDxInputPool();

		bool Initialize(PGAME game, bool mouseExclusive = false);
		bool UpdateDevice();

		int KeyUp(unsigned long key);
		int KeyDown(unsigned long key);

		int MouseButtonUp(unsigned long button);
		int MouseButtonDown(unsigned long button);

		POINT GetMousePos();
		POINT GetMousePosDelta();
		long GetMouseWheelPosDelta();

		void ClearUp();

	private:
		LPDIRECTINPUT8 mInput;

		LPDIRECTINPUTDEVICE8 mKeyboard;
		//����ʵ���ϲ��Ǵ洢�ַ���������Ҫһ��sizeof(char)���͵����� ǧ�����TCHAR
		char mKeys[KEYS_SIZE];
		char moldKeys[KEYS_SIZE];

		LPDIRECTINPUTDEVICE8 mMouse;
		DIMOUSESTATE mMouseState;
		DIMOUSESTATE moldMouseState;

		long mxMPos, myMPos;
		long mxMPosDelta, myMPosDelta, mzMPosDelta;
	};
}
