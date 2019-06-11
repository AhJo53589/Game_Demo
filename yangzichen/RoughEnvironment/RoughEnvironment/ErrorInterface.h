#pragma once

#include "define.h"

namespace pool
{
	class CDeviceErrorInterface
	{
	public:
		CDeviceErrorInterface(TCHAR *c);
		CDeviceErrorInterface(const CDeviceErrorInterface &c);
		~CDeviceErrorInterface();

		virtual void ErrorMessage(TCHAR *c, HWND hWnd = NULL);

	protected:
		TCHAR mClassname[50];
	};
}

