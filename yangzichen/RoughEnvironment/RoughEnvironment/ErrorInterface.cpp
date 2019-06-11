#include "ErrorInterface.h"

namespace pool
{
	CDeviceErrorInterface::CDeviceErrorInterface(TCHAR *c) 
	{
		lstrcpy(this->mClassname, c);
		lstrcat(this->mClassname, TEXT("-ERROR"));
	}

	CDeviceErrorInterface::CDeviceErrorInterface(const CDeviceErrorInterface &c)
	{
		lstrcpy(this->mClassname, c.mClassname);
	}


	CDeviceErrorInterface::~CDeviceErrorInterface(void)
	{
	}

	void CDeviceErrorInterface::ErrorMessage(TCHAR *c, HWND hWnd)
	{
		MessageBox(hWnd, c, this->mClassname, MB_OK);
	}
}
