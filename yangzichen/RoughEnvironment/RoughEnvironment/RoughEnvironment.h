#pragma once

#include "PoolEngine.h"

class CWorld : public pool::CNodePool
{
public:
	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
};

class CBox : public pool::CNodePool
{
public:
	CBox(float x, float y, float z)
	{
		this->mSpace.At(x, y, z);
	}
	~CBox()
	{
	}
	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
};