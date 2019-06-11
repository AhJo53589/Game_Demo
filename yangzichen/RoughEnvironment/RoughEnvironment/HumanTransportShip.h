#pragma once
#include "PoolEngine.h"

class CHumanTransportShipSail : public pool::CNodePool
{
public:
	CHumanTransportShipSail() : mR(0), mT(pool::DxCommon::Pi/16), mS(pool::DxCommon::Pi/32)
	{
	}
	CHumanTransportShipSail(float x, float y, float z)
	{
		this->mSpace.At(x, y, z);
	}
	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
private:
	float mR, mT, mS;
};

class CHumanTransportShip : public pool::CNodePool
{
public:
	CHumanTransportShip()
	{
	}
	CHumanTransportShip(float x, float y, float z)
	{
		this->mSpace.At(x, y, z);
	}
	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
};