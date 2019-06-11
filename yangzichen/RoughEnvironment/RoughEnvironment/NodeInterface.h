#pragma once

#include "define.h"
#include "InputInterface.h"
#include "CameraInterface.h"
#include "SpaceState.h"

namespace pool
{
	enum LIFE {
		LIFE_BORN,
		LIFE_LIFE,
		LIFE_SLEEP,
		LIFE_DING,
		LIFE_DEAD
	};
	
	class CNodeInterface
	{
	public:
		CNodeInterface()
		{
		}
		~CNodeInterface()
		{
		}

		virtual void Initialize(LPDIRECT3DDEVICE9 device, PCAMERA camera) = 0;
		virtual void Draw(LPDIRECT3DDEVICE9 device, PCAMERA camera, bool flag = false) = 0;
		virtual void BeforeDraw(LPDIRECT3DDEVICE9 device, PCAMERA camera, CNodeInterface *rootNode, PGAME game) = 0;
		virtual void Update(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera) = 0;
		virtual void Response(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera) = 0;
		virtual void Clear() = 0;
		virtual PSPACE GetSpace()
		{
			return &this->mSpace;
		}
		virtual void SetParentSpace(PSPACE parentSpace)
		{
			this->mSpace.mPatent = parentSpace;
		}
		virtual LIFE Life() = 0;

	protected:
		CSpaceState mSpace;
		
	};

	typedef CNodeInterface *PNODE;
}