#pragma once
#include "Util.h"
#include "Camera.h"

//正交投影的摄像机
class Camera2d : public Camera
{
public:
	Camera2d() {}
	virtual ~Camera2d(){}

	virtual void update(float dt) {}

	virtual void init(float width, float height, float Znear, float Zfar);

	virtual void enableCamera();

	virtual void at(float x, float y, float z = 0);
	virtual void move(float x, float y, float z = 0);
};

