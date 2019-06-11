#pragma once

#include "define.h"
#include "ErrorInterface.h"

namespace pool
{
	class CPixelShaderPool : public CDeviceErrorInterface
	{
	public:
		CPixelShaderPool() : CDeviceErrorInterface(TEXT("PixelShaderPool")), 
			mPixelShader(NULL), mTransformConstantTable(NULL)
		{
		}
		~CPixelShaderPool()
		{
			if (this->mPixelShader)
			{
				this->mPixelShader->Release();
				this->mPixelShader = NULL;
			}

			if (this->mTransformConstantTable)
			{
				this->mTransformConstantTable->Release();
				this->mTransformConstantTable = NULL;
			}
		}
		
		bool InitializePixelShader(LPDIRECT3DDEVICE9 device, TCHAR* file, CHAR* functionName, CHAR* target);

		IDirect3DPixelShader9* GetPixelShader()
		{
			return this->mPixelShader;
		}
		
		ID3DXConstantTable* GetTransformConstantTable()
		{
			return this->mTransformConstantTable;
		}

	private:
		IDirect3DPixelShader9* mPixelShader;
		ID3DXConstantTable* mTransformConstantTable;
	};
}