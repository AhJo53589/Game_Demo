#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <string>
#include <sstream>
#include <vector>
#include <Windows.h>
#include "GameCell.h"

#define ReleaseCOM(x) { if(x){ x->Release();x = NULL; } }
#define Delete(x) { if(x) { delete x; x = NULL; }}

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
		DXTrace(__FILE__, __LINE__, hr,NULL, TRUE); \
		}                                              \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif 

#define oC (D3DX_PI/180)

#define TEST3D_