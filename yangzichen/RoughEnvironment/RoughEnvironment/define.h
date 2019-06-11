#pragma once

#include <Windows.h>
#include <stdlib.h>
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <list>
#include <string>
#include <map>

#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")
#pragma comment(lib,"winmm.lib") 

namespace pool
{	
	//采多重样率
	namespace eMSL
	{
		enum MultiSampleLevel
		{
			p_MSL_0,
			p_MSL_2,
			p_MSL_4,
			p_MSL_8,
			p_MSL_16
		};
	}

	namespace DxCommon
	{
		const float oC(D3DX_PI/180) ;
		const float Pi(D3DX_PI) ;
		typedef D3DXCOLOR XCOLOR;
		typedef D3DXVECTOR3 V3;
		typedef D3DXVECTOR4 V4;
		typedef D3DXMATRIX M4X4;

		float GetRandom();

		struct Vertex_TEX1
		{
			Vertex_TEX1(){}
			Vertex_TEX1(float x, float y, float z,
				float nx, float ny, float nz,
				float u, float v)
			{
				mx  = x;  my  = y;  mz  = z;
				mnx = nx; mny = ny; mnz = nz;
				mu  = u;  mv  = v;
			}

			float mx, my, mz;
			float mnx, mny, mnz;
			float mu, mv;

			static const DWORD FVF;
		};

		struct Vertex
		{
			Vertex(){}
			Vertex(float x, float y, float z,
				float nx, float ny, float nz)
			{
				mx  = x;  my  = y;  mz  = z;
				mnx = nx; mny = ny; mnz = nz;
			}

			float mx, my, mz;
			float mnx, mny, mnz;
			
			static const DWORD FVF;
		};

		struct Vertex_Point
		{
			Vertex_Point(){}
			Vertex_Point(float x, float y, float z)
			{
				mx  = x;  my  = y;  mz  = z;
			}

			float mx, my, mz;
			
			static const DWORD FVF;
		};
	}
}