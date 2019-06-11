#pragma once

#include "define.h"
#include "math.h"

namespace pool
{
	namespace Primitive
	{
		bool DrawSphere(LPDIRECT3DDEVICE9 pDevice, float radius, unsigned int slices, unsigned int stacks, LPD3DXMESH *ppMesh, bool ccw = false);
	}
}