#pragma once

#include "PoolEngine.h"

class CSky : public pool::CNodePool
{
public:
	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
};