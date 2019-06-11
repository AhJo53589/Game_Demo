#pragma once
#include "camerainterface.h"

namespace pool
{
	class CCameraThirdPerson : public CCameraInterface
	{
	public:
		CCameraThirdPerson();
		~CCameraThirdPerson();

		//左、右
		void LRMove(float n); 
		//上、下
		void UDMove(float n); 
		//前、后
		void FBMove( float n);
		//X轴正方向旋转
		void XRotation(float angle);
		//Y轴正方向旋转
		void YRotation(float angle);
		//Z轴正方向旋转
		void ZRotation(float angle);

		void Initialize(float posX, float posY, float posZ,
			float lookX, float lookY, float lookZ,
			float upX, float upY, float upZ);
		void Initialize(D3DXVECTOR3 pos,
			D3DXVECTOR3 look,
			D3DXVECTOR3 up);
		void Initialize(D3DXVECTOR3 pos,
			D3DXVECTOR3 look,
			D3DXVECTOR3 up,
			D3DXVECTOR3 right);

		D3DXMATRIX GetViewMatrix();
		D3DXVECTOR3 GetPosition();
		void SetPosition(D3DXVECTOR3 pos);
		void SetPosition(float x, float y, float z);
		D3DXVECTOR3 GetRight();
		void SetRight(D3DXVECTOR3 pos);
		void SetRight(float x, float y, float z);
		D3DXVECTOR3 GetUp();
		void SetUp(D3DXVECTOR3 pos);
		void SetUp(float x, float y, float z);
		D3DXVECTOR3 GetLook();
		void SetLook(D3DXVECTOR3 pos);
		void SetLook(float x, float y, float z);

		void Response(DWORD time, PINPUT input);

	private:
		D3DXVECTOR3 mRight;
		D3DXVECTOR3 mUp;
		D3DXVECTOR3 mLook;
		D3DXVECTOR3 mPos;

		float mDistance;
	};
}

