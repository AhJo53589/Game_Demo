#pragma once
#include "PoolEngine.h"
#include "Sky.h"
#include "Sea.h"
#include "HumanTransportShip.h"

class CPlayer : public pool::CNodePool
{
public:
	CPlayer() : mDirection(1.0f, 0.0f, 0.0f), mSteeringSpeed(0.06), mSpeed(0.3)
	{
	}
	~CPlayer()
	{
	}

	void InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	void ResponseEvent(DWORD time, pool::PINPUT input, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);
	void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera);

private:
	pool::DxCommon::V3 mDirection;
	float mSteeringSpeed, mSpeed;
};