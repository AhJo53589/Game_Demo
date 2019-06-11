#pragma once

#include "define.h"
#include <string>

namespace pool
{
	class CTexture
	{
	public:
		CTexture() : mTexture(NULL), mTexturePath(TEXT("")), mOffScreenSurface(NULL), mWidth(0), mHeight(0)
		{
		}
		~CTexture()
		{
			if (this->mTexture)
			{
				this->mTexture->Release();
				this->mTexture = NULL;
			}

			if (this->mOffScreenSurface)
			{
				this->mOffScreenSurface->Release();
				this->mOffScreenSurface = NULL;
			}
		}

		bool CreateTextureFromFile(const TCHAR *c, LPDIRECT3DDEVICE9 device);

		bool CreateTexture(LPDIRECT3DDEVICE9 device,  int width, int height);

		bool CreateTextureForRender(LPDIRECT3DDEVICE9 device,  int width, int height);

		IDirect3DTexture9 *mTexture;
		LPDIRECT3DSURFACE9 mOffScreenSurface;
		std::wstring mTexturePath;
		int mWidth, mHeight;
	};
}