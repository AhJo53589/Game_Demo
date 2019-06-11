#pragma once
#include "Camera3d.h"
#include "Node.h"

class Camera3dUse : public Camera3d, public MovementEventListener
{
public:
	Camera3dUse() {}
	virtual ~Camera3dUse() {}

	virtual void TranslationEvent(Node *n, D3DXVECTOR3 t);
};