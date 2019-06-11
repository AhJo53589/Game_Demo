#pragma once
#include "Camera2d.h"
#include "Node.h"

class Camera2drpg : public Camera2d, public MovementEventListener
{
public:
	Camera2drpg() {}
	virtual ~Camera2drpg() {}

	virtual void TranslationEvent(Node *n, D3DXVECTOR3 t);
};

