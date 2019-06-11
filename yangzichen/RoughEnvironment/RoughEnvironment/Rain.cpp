#include "Rain.h"

void CRain::BeforeInitialzeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	this->SetParticleRange(0, 0, 0, 10, 20, 10);
	this->mCreateParticlesWithTime = 500;
	this->mMaxParticles = 1000;

	pool::DxCommon::XCOLOR mcolor1(0.9, 0.8, 0.8, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(0.7, 0.7, 0.7, 1.0f);
	this->mMaterial = pool::Light::InitMtrl(mcolor1, mcolor1, mcolor1, mcolor2, 10);

	this->mParticleSize = 0.6;
	this->mParticleMinSize = 0.2;

	this->mTexture = pool::LoadTexture(TEXT("Texture\\Raindrop.tga"), device);

	this->mSpace.mPatentRotationFlag = false;
}

void  CRain::ResponseEvent(pool::PINPUT input, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	if (input->KeyUp(DIK_R))
	{
		if (this->mLife == pool::LIFE_LIFE)
			this->mLife = pool::LIFE_SLEEP;
		else
			this->mLife = pool::LIFE_LIFE;
	}
}

void CRain::CreateParticlesEvent()
{
	pool::CParticle raindrop;
	raindrop.mPosition.x = 10 * pool::DxCommon::GetRandom();
	raindrop.mPosition.y = 0;
	raindrop.mPosition.z = 10 * pool::DxCommon::GetRandom();
	raindrop.mDirection.x = 0;
	raindrop.mDirection.y = 0;
	raindrop.mDirection.z = 0;
	raindrop.mAcceleration.x = 0;
	raindrop.mAcceleration.y = -0.01;
	raindrop.mAcceleration.z = 0;
	this->mParticleList.push_back(raindrop);
}

void CRain::UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	pool::DxCommon::V3 cpos= camera->GetPosition();
	this->mSpace.At(cpos.x, cpos.y + 10, cpos.z);
}