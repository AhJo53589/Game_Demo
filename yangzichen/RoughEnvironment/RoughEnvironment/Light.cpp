#include "Light.h"

namespace pool
{
	namespace Light
	{
		D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
		{
			D3DMATERIAL9 mtrl;
			
			//Ambient——指定此表面反射的环境光数量
			mtrl.Ambient  = a;
			//Diffuse——指定此表面反射的漫射光数量
			mtrl.Diffuse  = d;
			//Specular——指定此表面反射的镜面光数量
			mtrl.Specular = s;
			//Emissive——这个是被用来给表面添加颜色，它使得物体看起来就象是它自己发出的光一样
			mtrl.Emissive = e;
			//Power——指定锐利的镜面高光；它的值是高光的锐利值
			mtrl.Power    = p;
			return mtrl;
		}

		D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
		{
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(light));

			light.Type      = D3DLIGHT_DIRECTIONAL;
			light.Ambient   = *color * 0.6f;
			light.Diffuse   = *color;
			light.Specular  = *color * 0.6f;
			light.Direction = *direction;

			return light;
		}

		D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
		{
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(light));

			light.Type      = D3DLIGHT_POINT;
			light.Ambient   = *color * 0.6f;
			light.Diffuse   = *color;
			light.Specular  = *color * 0.6f;
			light.Position  = *position;
			light.Range        = 1000.0f;
			light.Falloff      = 1.0f;
			light.Attenuation0 = 1.0f;
			light.Attenuation1 = 0.0f;
			light.Attenuation2 = 0.0f;

			return light;
		}

		D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
		{
			D3DLIGHT9 light;
			ZeroMemory(&light, sizeof(light));

			light.Type      = D3DLIGHT_SPOT;
			light.Ambient   = *color * 0.0f;
			light.Diffuse   = *color;
			light.Specular  = *color * 0.6f;
			light.Position  = *position;
			light.Direction = *direction;
			light.Range        = 1000.0f;
			light.Falloff      = 1.0f;
			light.Attenuation0 = 1.0f;
			light.Attenuation1 = 0.0f;
			light.Attenuation2 = 0.0f;
			light.Theta        = 0.4f;
			light.Phi          = 0.9f;

			return light;
		}
	}
}
