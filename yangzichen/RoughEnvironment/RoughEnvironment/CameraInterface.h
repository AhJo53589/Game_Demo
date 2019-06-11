#pragma once

#include "define.h"
#include "InputDevice.h"

namespace pool
{
	class CCameraInterface
	{
	public:
		CCameraInterface()
		{
		}
		~CCameraInterface()
		{}

		//左、右
		virtual void LRMove(float n) = 0; 
		//上、下
		virtual void UDMove(float n) = 0; 
		//前、后
		virtual void FBMove( float n) = 0;
		//X轴正方向旋转
		virtual void XRotation(float angle) = 0;
		//Y轴正方向旋转
		virtual void YRotation(float angle) = 0;
		//Z轴正方向旋转
		virtual void ZRotation(float angle) = 0;


		virtual void Initialize(float posX, float posY, float posZ,
			float lookX, float lookY, float lookZ,
			float upX, float upY, float upZ) = 0;
		virtual void Initialize(D3DXVECTOR3 pos,
			D3DXVECTOR3 look,
			D3DXVECTOR3 up) = 0;

		virtual D3DXMATRIX GetViewMatrix() = 0;
		virtual D3DXVECTOR3 GetPosition() = 0;
		virtual void SetPosition(D3DXVECTOR3 pos) = 0;
		virtual void SetPosition(float x, float y, float z) = 0;
		virtual D3DXVECTOR3 GetRight() = 0;
		virtual D3DXVECTOR3 GetUp() = 0;;
		virtual D3DXVECTOR3 GetLook() = 0;

		//响应
		virtual void Response(DWORD time, PINPUT input)
		{
		}
		//更新
		virtual void Update(DWORD time)
		{
		}

	};

	typedef CCameraInterface *PCAMERA;
}