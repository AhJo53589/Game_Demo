#pragma once

#include "PoolEngine.h"

class CRain : public pool::CParticleSystemPool
{
public:
	CRain()
	{
	}
	~CRain()
	{
	}

	void BeforeInitialzeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	
	void ResponseEvent(pool::PINPUT input, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);

	void CreateParticlesEvent();

	void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
};