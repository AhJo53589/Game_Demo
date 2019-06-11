#include "CameraThirdPerson.h"

namespace pool
{
	CCameraThirdPerson::CCameraThirdPerson() : mPos(0.0f, 0.0f, 0.0f),
		mLook(0.0f, 0.0f, 1.0f), mUp(0.0f, 0.1f, 0.0f), mRight(1.0f, 0.0f, 0.0f), mDistance(40)
	{
	}


	CCameraThirdPerson::~CCameraThirdPerson()
	{
	}

	void CCameraThirdPerson::LRMove(float n)
	{
		this->mPos += this->mRight * n;
	}

	void CCameraThirdPerson::UDMove(float n)
	{
		this->mPos += this->mUp * n;
	}

	void CCameraThirdPerson::FBMove(float n)
	{
		this->mPos += this->mLook * n;
	}

	void CCameraThirdPerson::Initialize(float posX, float posY, float posZ,
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

	void CCameraThirdPerson::Initialize(D3DXVECTOR3 pos, D3DXVECTOR3 look, D3DXVECTOR3 up)
	{
		this->mPos = pos;

		this->mLook = look;

		this->mUp = up;

		D3DXVec3Normalize(&this->mLook, &this->mLook);
		D3DXVec3Normalize(&this->mUp, &this->mUp);
		D3DXVec3Cross(&this->mRight, &this->mLook, &this->mUp);
		D3DXVec3Normalize(&this->mRight, &this->mRight);
	}

	void CCameraThirdPerson::XRotation(float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &mRight, angle);

		//当向量通过该矩阵完成旋转及平移变换.
		D3DXVec3TransformCoord(&this->mUp,&this->mUp, &T);
		D3DXVec3TransformCoord(&this->mLook,&this->mLook, &T);
	}

	void CCameraThirdPerson::YRotation(float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationY(&T, angle);

		D3DXVec3TransformCoord(&this->mRight,&this->mRight, &T);
		D3DXVec3TransformCoord(&this->mLook,&this->mLook, &T);
	}

	void CCameraThirdPerson::ZRotation(float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &this->mLook,	angle);

		D3DXVec3TransformCoord(&this->mRight,&this->mRight, &T);
		D3DXVec3TransformCoord(&this->mUp,&this->mUp, &T);
	}

	void CCameraThirdPerson::SetPosition(D3DXVECTOR3 pos)
	{
		this->mPos = pos;
	}

	void CCameraThirdPerson::SetPosition(float x, float y, float z)
	{
		D3DXVECTOR3 pos(x, y, z);
		this ->mPos = pos;
	}

	D3DXVECTOR3 CCameraThirdPerson::GetPosition()
	{
		return this->mPos - this->mDistance * this->mLook;
	}

	D3DXVECTOR3 CCameraThirdPerson::GetLook()
	{
		return this->mLook;
	}

	D3DXVECTOR3 CCameraThirdPerson::GetRight()
	{
		return this->mRight;
	}

	D3DXVECTOR3 CCameraThirdPerson::GetUp()
	{
		return this->mUp;
	}

	D3DXMATRIX CCameraThirdPerson::GetViewMatrix()
	{
		D3DXMATRIX v;
		
		D3DXVec3Normalize(&this->mLook, &this->mLook);

		D3DXVec3Cross(&this->mUp, &this->mLook, &this->mRight);
		D3DXVec3Normalize(&this->mUp, &this->mUp);

		D3DXVec3Cross(&this->mRight, &this->mUp, &this->mLook);
		D3DXVec3Normalize(&this->mRight, &this->mRight);

		D3DXVECTOR3 at = this->mPos;
		D3DXVECTOR3 eye = this->mPos - this->mDistance * this->mLook;

		D3DXMatrixLookAtLH(&v, &eye, &at, &this->mUp);

		return v;
	}

	void CCameraThirdPerson::Response(DWORD time, PINPUT input)
	{	
		if (input->MouseButtonDown(Mouse::LEFT_BUTTON))
		{
			this->XRotation(- input->GetMousePosDelta().y * pool::DxCommon::oC / 10);
			this->YRotation(- input->GetMousePosDelta().x * pool::DxCommon::oC / 10);
		}
		
		this->mDistance -= 0.01 * input->GetMouseWheelPosDelta();

		if (this->mDistance < 5.0f)
			this->mDistance = 5.0f;
		else if (this->mDistance > 80.0f)
			this->mDistance = 80.0f;

	}
}
