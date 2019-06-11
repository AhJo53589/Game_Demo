#pragma once

#include "define.h"

namespace pool
{
	class CGameInterface
	{
	public:
		CGameInterface(int width = 640, int height = 480) : mWidth(width), mHeight(height), mBackBufferWidth(1024), mBackBufferHeight(768),
			mFullscreen(false) ,mhWnd(NULL)
		{
		}
		~CGameInterface()
		{
		}

		virtual bool Initialize(HWND hWnd, HINSTANCE hInstance)
		{
			if (!hWnd && !hInstance)
			{
				this->mhWnd = hWnd;
				this->mhInstance = hInstance;
				return true;
			}
			else
				return false;
		}

		virtual void Loop()
		{
		}

		HWND GethWnd()
		{
			return this->mhWnd;
		}

		HINSTANCE GethInstance()
		{
			return this->mhInstance;
		}

	public:
		int mWidth, mHeight;
		int mBackBufferWidth, mBackBufferHeight;
		bool mFullscreen;

	protected:
		HWND mhWnd;
		HINSTANCE mhInstance;

	};

	typedef CGameInterface *PGAME;
}