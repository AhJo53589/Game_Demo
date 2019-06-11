#pragma once
#include "define.h"
#include "Texture.h"
#include <string>
#include <map>

namespace pool
{
	void InitResManager(LPDIRECT3DDEVICE9 device);

	ID3DXMesh *LoadMesh(const TCHAR * c, LPDIRECT3DDEVICE9 device);

	CTexture *GetEmptyTexture();
	CTexture *LoadTexture(const TCHAR * c, LPDIRECT3DDEVICE9 device);
	CTexture *CreateTexture(const TCHAR * c, LPDIRECT3DDEVICE9 device);
}

