#include "define.h"

namespace pool
{	
	namespace DxCommon
	{
		const DWORD Vertex_TEX1::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

		const DWORD Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL;

		const DWORD Vertex_Point::FVF = D3DFVF_XYZ;

		float GetRandom()
		{
			return ((float)rand() - (float)rand()) / RAND_MAX;
		}
	}
}