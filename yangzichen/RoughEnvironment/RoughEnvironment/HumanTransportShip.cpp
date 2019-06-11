#include "HumanTransportShip.h"

void CHumanTransportShipSail::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	this->mMesh = pool::LoadMesh(TEXT("Xfile\\HumanTransportShipSail.X"), device);

	pool::DxCommon::XCOLOR mcolor1(0.4, 0.4, 0.4, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(0.3, 0.3, 0.3, 1.0f);
	this->mMaterial = pool::Light::InitMtrl(mcolor1, mcolor1, mcolor1, mcolor2, 10);

	this->mTexture = pool::LoadTexture(TEXT("Texture\\HumanDestroyerShip.TGA"), device);
}

void CHumanTransportShipSail::UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	pool::DxCommon::V3 R(0.0f, 1.0f, 0.0f);
	this->mR = this->mR + this->mT * time /1000;
	
	if ((this->mR > this->mS && this->mT > 0) ||
		 (this->mR < -this->mS && this->mT < 0))
		this->mT *= -1;

	if (this->mR > this->mS)
		this->mR = this->mS;
	if (this->mR < -this->mS)
		this->mR = -this->mS;

	this->mSpace.Rotation(R,  this->mR);
}

void CHumanTransportShip::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	this->mMesh = pool::LoadMesh(TEXT("Xfile\\HumanTransportShip.X"), device);
	this->mSpace.Size(0.05,0.05,0.05);

	pool::DxCommon::XCOLOR mcolor1(0.4, 0.4, 0.4, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(0.3, 0.3, 0.3, 1.0f);
	this->mMaterial = pool::Light::InitMtrl(mcolor1, mcolor1, mcolor1, mcolor2, 10);

	this->mTexture = pool::LoadTexture(TEXT("Texture\\HumanDestroyerShip.TGA"), device);

	this->mList.push_back(new CHumanTransportShipSail());
}
