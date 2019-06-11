#include "ResManager.h"

namespace pool
{
	std::map<std::wstring, pool::CTexture *> TextureList;
	std::map<std::wstring, ID3DXMesh *> MeshList;

	void InitResManager(LPDIRECT3DDEVICE9 device)
	{
		TextureList[TEXT("None")] = new CTexture();
		CTexture *t =new CTexture();
		t->CreateTextureFromFile(TEXT("Texture\\default.png"), device);
		TextureList[TEXT("Default")] = t;
	}

	void FiniResManager()
	{
	}
	
	ID3DXMesh *LoadMesh(const TCHAR * c, LPDIRECT3DDEVICE9 device)
	{
		std::wstring name(c);
		if (!MeshList.count(name))
		{
			ID3DXMesh *m = NULL;
			D3DXLoadMeshFromX(c, D3DXMESH_MANAGED, device, NULL, NULL, NULL, NULL, &m);
			MeshList[name] = m; 
		}
		return MeshList[name];
	}

	CTexture *LoadTexture(const TCHAR * c, LPDIRECT3DDEVICE9 device)
	{
		std::wstring name(c);
		if (!TextureList.count(name))
		{
			CTexture *t =new CTexture();

			if (!t->CreateTextureFromFile(c, device))
			{
				delete t;
				return TextureList[TEXT("Default")];
			}
			TextureList[name] = t;
		}

		return TextureList[name];
	}

	CTexture *CreateTexture(const TCHAR * c, LPDIRECT3DDEVICE9 device)
	{
		std::wstring name(c);
		if (!TextureList.count(name))
		{
			CTexture *t =new CTexture();
			TextureList[name] = t;
		}

		return  TextureList[name];
	}

	CTexture *GetEmptyTexture()
	{
		return TextureList[TEXT("None")];
	}
}