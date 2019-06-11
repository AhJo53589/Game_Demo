#pragma once
#include "Util.h"
#include "Camera.h"

//正交投影的摄像机
class Camera3d : public Camera
{
public:
	Camera3d() : Camera(), mDistance(0){}
	virtual ~Camera3d(){}

	virtual void update(float dt) {}

	virtual void init(float fovy, float Aspect, float zn, float zf);

	virtual void setCameraFree(D3DXVECTOR3 look, D3DXVECTOR3 up, D3DXVECTOR3 pos);
	virtual void setCameraLock(D3DXVECTOR3 look, D3DXVECTOR3 up, D3DXVECTOR3 pos, float length);

	virtual void enableCamera();

	virtual void at(float x, float y, float z);
	virtual void move(float x, float y, float z);

	virtual void trunX(float angle);
	virtual void trunY(float angle);
	virtual void trunZ(float angle);

	float getDistance() { return mDistance; }
	void setDistance(float l) { mDistance = l; }

protected:
	D3DXVECTOR3 mRight;
	D3DXVECTOR3 mUp;
	D3DXVECTOR3 mLook;

	float mDistance;
};