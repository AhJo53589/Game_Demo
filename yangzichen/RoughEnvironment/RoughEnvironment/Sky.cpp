#include "Sky.h"

void CSky::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	pool::Primitive::DrawSphere(device, 800.0f, 80, 80, &this->mMesh);
	pool::DxCommon::XCOLOR mcolor1(0.7, 0.7, 0.8, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(0.7, 0.7, 0.8, 1.0f);
	this->mMaterial = pool::Light::InitMtrl(mcolor1, mcolor1, mcolor1, mcolor2, 10);

	this->mTexture = pool::LoadTexture(TEXT("Texture\\Sky.jpg"), device);

	this->mSpace.mPatentRotationFlag = false;
}