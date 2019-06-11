#pragma once

#include "define.h"

namespace pool
{
	namespace Light
	{	
		typedef D3DMATERIAL9 MATERIAL;
		
		D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

		typedef D3DLIGHT9 LIGHT;

		D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
		D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
		D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

		
	}

}

