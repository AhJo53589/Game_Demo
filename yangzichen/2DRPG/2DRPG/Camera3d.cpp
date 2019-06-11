#include "Camera3d.h"
#include "Define2DRPG.h"

void Camera3d::init(float fovy, float Aspect, float zn, float zf)
{
	D3DXMatrixPerspectiveFovLH(&this->mMatProject, fovy, Aspect, zn, zf);
	this->setCameraFree(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, 0));
}

void Camera3d::setCameraFree(D3DXVECTOR3 look, D3DXVECTOR3 up, D3DXVECTOR3 pos)
{
	this->mPos = pos;

	this->mLook = look;

	this->mUp = up;

	D3DXVec3Normalize(&this->mLook, &this->mLook);
	D3DXVec3Normalize(&this->mUp, &this->mUp);
	D3DXVec3Cross(&this->mRight, &this->mLook, &this->mUp);
	D3DXVec3Normalize(&this->mRight, &this->mRight);

	this->mDistance = 0.0f;
}

void Camera3d::setCameraLock(D3DXVECTOR3 look, D3DXVECTOR3 up, D3DXVECTOR3 pos, float length)
{
	this->mPos = pos;

	this->mLook = look;

	this->mUp = up;

	D3DXVec3Normalize(&this->mLook, &this->mLook);
	D3DXVec3Normalize(&this->mUp, &this->mUp);
	D3DXVec3Cross(&this->mRight, &this->mLook, &this->mUp);
	D3DXVec3Normalize(&this->mRight, &this->mRight);

	this->mDistance = length;
}

void Camera3d::enableCamera()
{
	d3dDevice->SetTransform(D3DTS_PROJECTION, &this->mMatProject);
	if (mDistance != 0.0f)
	{
		D3DXVec3Normalize(&this->mLook, &this->mLook);

		D3DXVec3Cross(&this->mUp, &this->mLook, &this->mRight);
		D3DXVec3Normalize(&this->mUp, &this->mUp);

		D3DXVec3Cross(&this->mRight, &this->mUp, &this->mLook);
		D3DXVec3Normalize(&this->mRight, &this->mRight);

		D3DXVECTOR3 at = this->mPos;
		D3DXVECTOR3 eye = this->mPos - this->mDistance * this->mLook;

		D3DXMatrixLookAtLH(&this->mMatView, &eye, &at, &this->mUp);
	}
	else
	{
		D3DXVec3Normalize(&this->mLook, &this->mLook);

		D3DXVec3Cross(&this->mUp, &this->mLook, &this->mRight);
		D3DXVec3Normalize(&this->mUp, &this->mUp);

		D3DXVec3Cross(&this->mRight, &this->mUp, &this->mLook);
		D3DXVec3Normalize(&this->mRight, &this->mRight);

		D3DXVECTOR3 at = this->mPos + this->mLook;
		D3DXMatrixLookAtLH(&this->mMatView, &this->mPos, &at, &this->mUp);
	}
	d3dDevice->SetTransform(D3DTS_VIEW, &this->mMatView);
}

void Camera3d::at(float x, float y, float z)
{
	this->mPos[0] = x;
	this->mPos[1] = y;
	this->mPos[2] = z;

	this->notifyCameraMoveEvent();
}

void Camera3d::move(float x, float y, float z)
{
	this->mPos[0] += x;
	this->mPos[1] += y;
	this->mPos[2] += z;

	this->notifyCameraMoveEvent();
}

void Camera3d::trunX(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &mRight, angle * oC);

	D3DXVec3TransformCoord(&this->mUp,&this->mUp, &T);
	D3DXVec3TransformCoord(&this->mLook,&this->mLook, &T);
}

void Camera3d::trunY(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationY(&T, angle * oC);

	D3DXVec3TransformCoord(&this->mRight,&this->mRight, &T);
	D3DXVec3TransformCoord(&this->mLook,&this->mLook, &T);
}

void Camera3d::trunZ(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &this->mLook, angle * oC);

	D3DXVec3TransformCoord(&this->mRight,&this->mRight, &T);
	D3DXVec3TransformCoord(&this->mUp,&this->mUp, &T);
}