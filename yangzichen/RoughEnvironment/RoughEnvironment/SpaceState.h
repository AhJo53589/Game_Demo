#pragma once

#include "define.h"

namespace pool
{
	class CSpaceState
	{
	public:
		CSpaceState();
		~CSpaceState();

		void At(float x, float y, float z);
		void Move(float x, float y, float z);

		void Rotation(D3DXVECTOR3 axis, float angle);
		void Rotation(D3DXMATRIX matrix);
		void Turn(D3DXVECTOR3 axis, float angle);
		void Turn(D3DXMATRIX matrix);

		void Scaling(float x, float y, float z);
		void Size(float x, float y, float z);

		D3DXMATRIX GetRotationMatrix();
		D3DXMATRIX GetMoveMatrix();
		D3DXMATRIX GetSizeMatrix();


		D3DXMATRIX GetMatrix();
 
		D3DXMATRIX mRotationMatrix;

		CSpaceState* mPatent;

		bool mPatentRotationFlag;

		D3DXVECTOR3 mT, mS;
	};

	typedef CSpaceState *PSPACE;
}

