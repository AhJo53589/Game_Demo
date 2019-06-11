#include "Camera3dUse.h"
#include "Define2DRPG.h"

void Camera3dUse::TranslationEvent(Node *n, D3DXVECTOR3 t)
{
	D3DXVECTOR3 v3 = n->getMoveVec3();

	if (this->mDistance == 0)
	{
		this->at(v3[0], v3[1], v3[2]-500);
	}
	else
	{
		this->at(v3[0], v3[1], v3[2]);
	}
}