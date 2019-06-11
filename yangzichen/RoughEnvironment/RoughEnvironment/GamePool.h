#pragma once

#include "gameinterface.h"
#include "define.h"
#include "ErrorInterface.h"
#include "Renderer.h"
#include "InputDevice.h"
#include "Node.h"
#include "Console.h"

namespace pool
{
	class CGamePool : public CGameInterface, public CDeviceErrorInterface
	{
	public:
		CGamePool();
		~CGamePool();

		bool Initialize(HWND hWnd, HINSTANCE hInstance);
		void Loop();
		void ClearUp();
		void SetNode(PNODE node);
		void SetCamera(PCAMERA camera);

		DWORD GetTimeDelta();

	private:
		PRENDERER mRenderer;
		PINPUT mInput;
		PCAMERA mCamera;
		PNODE mNode;

		DWORD mTime;
		DWORD moldTime;
	};
}

