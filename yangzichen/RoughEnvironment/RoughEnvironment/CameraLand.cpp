#include "CameraLand.h"

namespace pool
{
	CCameraLand::CCameraLand() : mPos(0.0f, 15.0f, 0.0f), mLook(0.0f, 0.0f, 1.0f), mUp(0.0f, 0.1f, 0.0f), mRight(1.0f, 0.0f, 0.0f)
	{
	}


	CCameraLand::~CCameraLand()
	{
	}

	void CCameraLand::LRMove(float n)
	{
		this->mPos += this->mRight * n;
	}

	void CCameraLand::UDMove(float n)
	{
		this->mPos += this->mUp * n;
	}

	void CCameraLand::FBMove(float n)
	{
		this->mPos += this->mLook * n;
	}

	void CCameraLand::Initialize(float posX, float posY, float posZ,
			float lookX, float lookY, float lookZ,
			float upX, float upY, float upZ)
	{
		D3DXVECTOR3 pos(posX, posY, posZ);
		this->mPos = pos;

		D3DXVECTOR3 look(lookX, lookY, lookZ);
		this->mLook = look;

		D3DXVECTOR3 up(lookX, lookY, lookZ);
		this->mUp = up;

		D3DXVec3Normalize(&this->mLook, &this->mLook);
		D3DXVec3Normalize(&this->mUp, &this->mUp);
		D3DXVec3Cross(&this->mRight, &this->mLook, &this->mUp);
		D3DXVec3Normalize(&this->mRight, &this->mRight);
	}

	void CCameraLand::Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 up)
	{
		this->mPos = pos;

		this->mLook = look;

		this->mUp = up;

		D3DXVec3Normalize(&this->mLook, &this->mLook);
		D3DXVec3Normalize(&this->mUp, &this->mUp);
		D3DXVec3Cross(&this->mRight, &this->mLook, &this->mUp);
		D3DXVec3Normalize(&this->mRight, &this->mRight);
	}

	void CCameraLand::XRotation(float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &mRight, angle);

		//当向量通过该矩阵完成旋转及平移变换.
		D3DXVec3TransformCoord(&this->mUp,&this->mUp, &T);
		D3DXVec3TransformCoord(&this->mLook,&this->mLook, &T);
	}

	void CCameraLand::YRotation(float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationY(&T, angle);

		D3DXVec3TransformCoord(&this->mRight,&this->mRight, &T);
		D3DXVec3TransformCoord(&this->mLook,&this->mLook, &T);
	}

	void CCameraLand::ZRotation(float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &this->mLook,	angle);

		D3DXVec3TransformCoord(&this->mRight,&this->mRight, &T);
		D3DXVec3TransformCoord(&this->mUp,&this->mUp, &T);
	}

	void CCameraLand::SetPosition(D3DXVECTOR3 pos)
	{
		this->mPos = pos;
	}

	void CCameraLand::SetPosition(float x, float y, float z)
	{
		D3DXVECTOR3 pos(x, y, z);
		this ->mPos = pos;
	}

	D3DXVECTOR3 CCameraLand::GetPosition()
	{
		return this->mPos;
	}

	D3DXVECTOR3 CCameraLand::GetLook()
	{
		return this->mLook;
	}

	D3DXVECTOR3 CCameraLand::GetRight()
	{
		return this->mRight;
	}

	D3DXVECTOR3 CCameraLand::GetUp()
	{
		return this->mUp;
	}

	D3DXMATRIX CCameraLand::GetViewMatrix()
	{
		D3DXMATRIX v;
		
		D3DXVec3Normalize(&this->mLook, &this->mLook);

		D3DXVec3Cross(&this->mUp, &this->mLook, &this->mRight);
		D3DXVec3Normalize(&this->mUp, &this->mUp);

		D3DXVec3Cross(&this->mRight, &this->mUp, &this->mLook);
		D3DXVec3Normalize(&this->mRight, &this->mRight);

		/* 不使用D3DXMatrixLookAtLH的原因
			D3DXMATRIX * D3DXMatrixLookAtLH(
				D3DXMATRIX * pOut,
				CONST D3DXVECTOR3 * pEye,
				CONST D3DXVECTOR3 * pAt,
				CONST D3DXVECTOR3 * pUp
			);
 
			zaxis = normal(At - Eye)
			xaxis = normal(cross(Up, zaxis))
			yaxis = cross(zaxis, xaxis)
   
			xaxis.x					yaxis.x					zaxis.x					0
			xaxis.y					yaxis.y					zaxis.y					0
			xaxis.z					yaxis.z					zaxis.z					0
			-dot(xaxis, eye)		-dot(yaxis, eye)		-dot(zaxis, eye)		l
		*/

		float x = -D3DXVec3Dot(&this->mRight, &this->mPos);
		float y = -D3DXVec3Dot(&this->mUp, &this->mPos);
		float z = -D3DXVec3Dot(&this->mLook, &this->mPos);

		v(0,0) = this->mRight.x; v(0, 1) = this->mUp.x; v(0, 2) =this->mLook.x; v(0, 3) = 0.0f;
		v(1,0) =this->mRight.y; v(1, 1) = this->mUp.y; v(1, 2) = this->mLook.y; v(1, 3) = 0.0f;
		v(2,0) = this->mRight.z; v(2, 1) = this->mUp.z; v(2, 2) = this->mLook.z; v(2, 3) = 0.0f;
		v(3,0) = x;        v(3, 1) = y;     v(3, 2) = z;       v(3, 3) = 1.0f;

		return v;
	}

	void CCameraLand::Response(DWORD time, PINPUT input)
	{	
		if (input->MouseButtonDown(Mouse::LEFT_BUTTON))
		{
			this->XRotation(- input->GetMousePosDelta().y * pool::DxCommon::oC / 10);
			this->YRotation(- input->GetMousePosDelta().x * pool::DxCommon::oC / 10);
		}
		
	}
}
