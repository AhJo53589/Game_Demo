#include "RoughEnvironment.h"

#include "Player.h"
#include "HumanTransportShip.h"
#include "Rain.h"

void CWorld::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	//pool::DxCommon::XCOLOR lcolor(1.0f, 1.0f, 1.0f, 1.0f);
	pool::DxCommon::XCOLOR lcolor(1.0f, 0.3f, 0.2f, 1.0f);
	pool::DxCommon::V3 v(0.0f, -1.0f, 0.0f);
	pool::Light::LIGHT light = pool::Light::InitDirectionalLight(&v, &lcolor);

	device->SetLight(0, &light);
	device->LightEnable(0, true);

	this->mList.push_back(new CPlayer());

	for (int n=1 ; n<10 ;  n+=2)
	{
		for (int m=2 ; m<10 ; m+=2)
		{
			this->mList.push_back(new CHumanTransportShip(100 * n, 0, 100 * m));
			this->mList.push_back(new CHumanTransportShip(100 * n, 0, -100 * m));
			this->mList.push_back(new CHumanTransportShip(-100 * n, 0, 100 * m));
			this->mList.push_back(new CHumanTransportShip(-100 * n, 0, -100 * m));
		}
	}
	for (int n=1 ; n<10 ;  n+=2)
	{
		for (int m=1 ; m<10 ; m+=2)
		{
			this->mList.push_back(new CBox(100 * n, 0, 100 * m));
			this->mList.push_back(new CBox(100 * n, 0, -100 * m));
			this->mList.push_back(new CBox(-100 * n, 0, 100 * m));
			this->mList.push_back(new CBox(-100 * n, 0, -100 * m));
		}
	}

	
	this->mList.push_back(new CRain());
}



void CBox::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	this->mMesh = pool::LoadMesh(TEXT("Xfile\\DalaranBuildings.X"), device);
	this->mSpace.Size(0.06,0.06,0.06);
	//this->mSpace.Scaling(1.5, 1.5, 0.2);

	/*pool::DxCommon::XCOLOR mcolor1(0.4, 0.4, 0.4, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(0.3, 0.3, 0.3, 1.0f);*/
	pool::DxCommon::XCOLOR mcolor1(0.3, 0.3, 0.3, 1.0f);
	pool::DxCommon::XCOLOR mcolor2(0.6, 0.6, 0.6, 1.0f);
	this->mMaterial = pool::Light::InitMtrl(mcolor1, mcolor1, mcolor1, mcolor2, 10);

	this->mTexture = pool::LoadTexture(TEXT("Texture\\DalaranBuildings.TGA"), device);
}

void CBox::UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	pool::DxCommon::V3 R(0.0f, 1.0f, 0.0f);
	this->mSpace.Turn(R, 15 * pool::DxCommon::oC*time/1000);
}