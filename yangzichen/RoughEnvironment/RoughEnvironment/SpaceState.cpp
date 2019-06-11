#include "SpaceState.h"

namespace pool
{
	CSpaceState::CSpaceState() : mT(0.0f, 0.0f, 0.0f), mS(1.0f, 1.0f, 1.0f), mPatent(NULL), mPatentRotationFlag(true)
	{
		D3DXMatrixIdentity(&this->mRotationMatrix);
	}


	CSpaceState::~CSpaceState()
	{
	}

	void CSpaceState::At(float x, float y, float z)
	{
		this->mT[0] = x;
		this->mT[1] = y;
		this->mT[2] = z;
	}

	void CSpaceState::Move(float x, float y, float z)
	{
		this->mT[0] += x;
		this->mT[1] += y;
		this->mT[2] += z;
	}

	void CSpaceState::Rotation(D3DXVECTOR3 axis, float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &axis, angle);

		this->mRotationMatrix = T;
	}
	
	void CSpaceState::Rotation(D3DXMATRIX matrix)
	{
		this->mRotationMatrix = matrix;
	}

	void CSpaceState::Turn(D3DXVECTOR3 axis, float angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &axis, angle);

		this->mRotationMatrix *= T;
	}

	void CSpaceState::Turn(D3DXMATRIX matrix)
	{
		this->mRotationMatrix *= matrix;
	}
	
	void CSpaceState::Scaling(float x, float y, float z)
	{
		this->mS[0] *= x;
		this->mS[1] *= y;
		this->mS[2] *= z;
	}

	void CSpaceState::Size(float x, float y, float z)
	{
		this->mS[0] = x;
		this->mS[1] = y;
		this->mS[2] = z;
	}

	D3DXMATRIX CSpaceState::GetRotationMatrix()
	{
		return this->mRotationMatrix;
	}

	D3DXMATRIX CSpaceState::GetMoveMatrix()
	{

		D3DXMATRIX t;
		D3DXMatrixTranslation(&t, this->mT[0], this->mT[1], this->mT[2]);

		return t;
	}

	D3DXMATRIX CSpaceState::GetSizeMatrix()
	{

		D3DXMATRIX t;
		D3DXMatrixScaling(&t, this->mS[0], this->mS[1], this->mS[2]);

		return t;
	}

	D3DXMATRIX CSpaceState::GetMatrix()
	{
		D3DXMATRIX matrix = this->GetSizeMatrix();

		matrix *= this->GetRotationMatrix();
		
		matrix *= this->GetMoveMatrix();

		if (this->mPatent)
		{
			if (this->mPatentRotationFlag)
			{
				return   matrix * this->mPatent->GetMatrix();
			}
			else
			{
				return   matrix * this->mPatent->GetSizeMatrix() * this->mPatent->GetMoveMatrix();
			}

		}

		return  matrix;
	}
}
