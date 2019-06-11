#include "Texture.h"

namespace pool
{
	bool CTexture::CreateTextureFromFile(const TCHAR *c, LPDIRECT3DDEVICE9 device)
	{
		if (!c)
			return false;
		this->mTexturePath = c;

		if (this->mTexture)
		{
			this->mTexture->Release();
			this->mTexture = NULL;
		}

		if (FAILED(D3DXCreateTextureFromFile(device, c, &this->mTexture)))
		{
			this->mTexturePath = TEXT("");
			return false;
		}

		return true;
	}

	bool CTexture::CreateTexture(LPDIRECT3DDEVICE9 device,  int width, int height)
	{
		if (this->mTexture)
		{
			this->mTexture->Release();
			this->mTexture = NULL;
		}

		if (D3DXCreateTexture(device, width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &this->mTexture))
		{
			this->mTexturePath = TEXT("");
			return false;
		}

		this->mWidth = width;
		this->mHeight = height;

		this->mTexturePath = TEXT("Create");
		return true;
	}

	bool CTexture::CreateTextureForRender(LPDIRECT3DDEVICE9 device,  int width, int height)
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

		if (D3DXCreateTexture(device, width, height, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &this->mTexture))
		{
			this->mTexturePath  = TEXT("");
			return false;
		}

		this->mWidth = width;
		this->mHeight = height;

		this->mTexture->GetSurfaceLevel(0, &this->mOffScreenSurface);

		this->mTexturePath = TEXT("Create");
		return true;
	}
}