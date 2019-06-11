#include "PixelShaderPool.h"

namespace pool
{
	bool CPixelShaderPool::InitializePixelShader(LPDIRECT3DDEVICE9 device, TCHAR* file, CHAR* functionName, CHAR* target)
	{
		ID3DXBuffer* shader = NULL;
		ID3DXBuffer* errorBuffer = NULL;
		
		//���ļ�������ɫ�� �ļ���, ��ѡ����, ID3DXInclude �ӿ�ָ��, ��ں���, ʹ��HLSL�汾, ������, 
		// �������ɫ�������ID3DXBuffer ָ��, ����������ʹ�����Ϣ�ַ�����ID3DXBuffer ָ��,
		// ��������ɫ�����������ݵ�ID3DXConstantTable ָ��
		if (FAILED(D3DXCompileShaderFromFile(file, 0, 0, functionName, target, D3DXSHADER_DEBUG,
			&shader, &errorBuffer, &this->mTransformConstantTable)))
		{
			TCHAR error[50] = {TEXT("���ļ�����Shaderʧ�� (")};
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
			this->ErrorMessage(TEXT("����Shaderʧ��"));
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