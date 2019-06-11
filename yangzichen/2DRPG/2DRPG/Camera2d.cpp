#include "Camera2d.h"
#include "Define2DRPG.h"

void Camera2d::init(float width, float height, float Znear, float Zfar)
{
	D3DXMatrixOrthoLH(&this->mMatProject, width, height, Znear, Zfar);
}

void Camera2d::enableCamera()
{
	d3dDevice->SetTransform(D3DTS_PROJECTION, &this->mMatProject);
	d3dDevice->SetTransform(D3DTS_VIEW, &this->mMatView);
}

void Camera2d::at(float x, float y, float z)
{
	this->mPos[0] = x;
	this->mPos[1] = y;
	D3DXVECTOR3 at = this->mPos;
	at[2] = 500;
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&this->mMatView, &this->mPos, &at, &up);

	this->notifyCameraMoveEvent();
}

void Camera2d::move(float x, float y, float z)
{
	this->mPos[0] += x;
	this->mPos[1] += y;
	D3DXVECTOR3 at = this->mPos;
	at[2] = -500;
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);

	D3DXMatrixLookAtLH(&this->mMatView, &this->mPos, &at, &up);

	this->notifyCameraMoveEvent();
}