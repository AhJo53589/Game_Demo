#include "Camera2drpg.h"
#include "Define2DRPG.h"

#define CAMERA_MAPSIZE_L (- (TILESIDE * WORLDMAPWIDTH / 2 - RESOLUTIONW / 2))
#define CAMERA_MAPSIZE_R (  (TILESIDE * WORLDMAPWIDTH / 2 - RESOLUTIONW / 2))
#define CAMERA_MAPSIZE_U (- (TILESIDE * WORLDMAPHEIGHT / 2 - RESOLUTIONH / 2))
#define CAMERA_MAPSIZE_D (  (TILESIDE * WORLDMAPHEIGHT / 2 - RESOLUTIONH / 2))

void Camera2drpg::TranslationEvent(Node *n, D3DXVECTOR3 t)
{
	D3DXVECTOR3 v3 = n->getMoveVec3();

	if (v3[0] < CAMERA_MAPSIZE_L)
		v3[0] = CAMERA_MAPSIZE_L;
	if (v3[0] > CAMERA_MAPSIZE_R)
		v3[0] = CAMERA_MAPSIZE_R;
	if (v3[1] < CAMERA_MAPSIZE_U)
		v3[1] = CAMERA_MAPSIZE_U;
	if (v3[1] > CAMERA_MAPSIZE_D)
		v3[1] = CAMERA_MAPSIZE_D;

	this->at(v3[0], v3[1]);
}
