#include "Player.h"

void CPlayer::InitializeEvent(LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	this->mList.push_back(new CSky);
	this->mList.push_back(new CSea);
	this->mList.push_back(new CHumanTransportShip());
}

void CPlayer::ResponseEvent(DWORD time, pool::PINPUT input, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	D3DXMATRIX t;

	if (input->KeyDown(DIK_RIGHT))
	{
		D3DXMatrixRotationY(&t, this->mSteeringSpeed * pool::DxCommon::oC * time);
		D3DXVec3TransformCoord(&this->mDirection,&this->mDirection, &t);
		this->mSpace.Turn(t);
	}

	if (input->KeyDown(DIK_LEFT))
	{
		D3DXMatrixRotationY(&t, -this->mSteeringSpeed * pool::DxCommon::oC * time);
		D3DXVec3TransformCoord(&this->mDirection,&this->mDirection, &t);
		this->mSpace.Turn(t);
	}

	D3DXVec3Normalize(&this->mDirection, &this->mDirection);

	float l = this->mSpeed * time;

	if (input->KeyDown(DIK_UP))
	{
		this->mSpace.Move(this->mDirection.x * l, this->mDirection.y * l, this->mDirection.z * l);
	}

	if (input->KeyDown(DIK_DOWN))
	{
		this->mSpace.Move(-this->mDirection.x * l, -this->mDirection.y * l, -this->mDirection.z * l);
	}

}

void CPlayer::UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, pool::PCAMERA camera)
{
	camera->SetPosition(this->mSpace.mT[0], this->mSpace.mT[1] + 10, this->mSpace.mT[2]);
}