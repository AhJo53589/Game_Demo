#pragma once
#include "Util.h"

class Camera;

//������۲���
class CameraMoveEventListener
{
public:
	//����������ƶ��¼�
	virtual void CameraMoveEvent(Camera *c, D3DXVECTOR3 pos) = 0;
};

class Camera
{
public:
	Camera() : mPos(0.0f, 0.0f, -500.0f) {}
	virtual ~Camera(){}

	virtual void update(float dt) {}

	virtual void at(float x, float y, float z = 0) {}
	virtual void move(float x, float y, float z = 0) {}

	virtual D3DXVECTOR3 getPos() { return this->mPos; }

	virtual void enableCamera() {}

	void addListener(CameraMoveEventListener *l);
    void removeListener(CameraMoveEventListener *l);

protected:
	D3DXMATRIX mMatProject;
	D3DXMATRIX mMatView;
	D3DXVECTOR3 mPos;

	std::vector<CameraMoveEventListener *> mListenerList;
	//֪ͨ����������ƶ�
	void notifyCameraMoveEvent();
};