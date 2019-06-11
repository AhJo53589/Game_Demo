#pragma once
#include "PoolEngine.h"

class CSea : public pool::CNodePool
{
public:
	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	void BeforeDrawEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera, pool::PNODE rootNode, pool::PGAME game);
	void DrawEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);

private:
	pool::DxCommon::Vertex_TEX1 mVertex[4];

	pool::CVertexShaderPool mVS;
	pool::CPixelShaderPool mPS;
};