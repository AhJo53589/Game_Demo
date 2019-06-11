#pragma once

#include "define.h"
#include "ErrorInterface.h"

namespace pool
{
	class CVertexShaderPool : public CDeviceErrorInterface
	{
	public:
		CVertexShaderPool() : CDeviceErrorInterface(TEXT("VertexShaderPool")), 
			mVertexShader(NULL), mTransformConstantTable(NULL)
		{
		}
		~CVertexShaderPool()
		{
			if (this->mVertexShader)
			{
				this->mVertexShader->Release();
				this->mVertexShader = NULL;
			}

			if (this->mTransformConstantTable)
			{
				this->mTransformConstantTable->Release();
				this->mTransformConstantTable = NULL;
			}
		}
		
		bool InitializeVertexShader(LPDIRECT3DDEVICE9 device, TCHAR* file, CHAR* functionName, CHAR* target);

		IDirect3DVertexShader9* GetVertexShader()
		{
			return this->mVertexShader;
		}
		
		ID3DXConstantTable* GetTransformConstantTable()
		{
			return this->mTransformConstantTable;
		}

	private:
		IDirect3DVertexShader9* mVertexShader;
		ID3DXConstantTable* mTransformConstantTable;
	};
}