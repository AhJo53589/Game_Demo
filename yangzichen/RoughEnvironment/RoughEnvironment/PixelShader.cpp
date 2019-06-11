#include "PixelShaderPool.h"

namespace pool
{
	bool CPixelShaderPool::InitializePixelShader(LPDIRECT3DDEVICE9 device, TCHAR* file, CHAR* functionName, CHAR* target)
	{
		ID3DXBuffer* shader = NULL;
		ID3DXBuffer* errorBuffer = NULL;
		
		//从文件编译着色器 文件名, 可选参数, ID3DXInclude 接口指针, 入口函数, 使用HLSL版本, 编译标记, 
		// 编译的着色器代码的ID3DXBuffer 指针, 包含错误码和错误消息字符串的ID3DXBuffer 指针,
		// 包含此着色器常量表数据的ID3DXConstantTable 指针
		if (FAILED(D3DXCompileShaderFromFile(file, 0, 0, functionName, target, D3DXSHADER_DEBUG,
			&shader, &errorBuffer, &this->mTransformConstantTable)))
		{
			TCHAR error[50] = {TEXT("从文件编译Shader失败 (")};
			lstrcat(error, file);
			lstrcat(error, TEXT(")"));
			this->ErrorMessage(error);
			if (errorBuffer)
			{
				TCHAR errormessage[200];
				MultiByteToWideChar(0, 0, (CHAR *)errorBuffer->GetBufferPointer(), -1, errormessage, 200);
				this->ErrorMessage(errormessage);
			}
			return false;
		}

		if (errorBuffer)
		{
			TCHAR errormessage[200];
			MultiByteToWideChar(0, 0, (CHAR *)errorBuffer->GetBufferPointer(), -1, errormessage, 200);
			this->ErrorMessage((TCHAR *)errorBuffer->GetBufferPointer());
			return false;
		}

		if (FAILED(device->CreatePixelShader((DWORD*)shader->GetBufferPointer(), &this->mPixelShader)))
		{
			this->ErrorMessage(TEXT("创建Shader失败"));
			return false;
		}

		if (shader)
		{
			shader->Release();
			shader = NULL;
		}

		if (errorBuffer)
		{
			errorBuffer->Release();
			errorBuffer = NULL;
		}

		return true;
	}
}