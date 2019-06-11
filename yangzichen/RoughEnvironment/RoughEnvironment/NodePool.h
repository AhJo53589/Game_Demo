#pragma once

#include "nodeinterface.h"
#include "define.h"
#include "CameraInterface.h"
#include "Texture.h"
#include "ResManager.h"

namespace pool
{
	class CNodePool : public CNodeInterface
	{
	public:
		CNodePool(); 
		~CNodePool();

		virtual void InitializeEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{

		}

		void Initialize(LPDIRECT3DDEVICE9 device, PCAMERA camera);

		virtual void BeforeDrawEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera, PNODE rootNode, PGAME game)
		{
		}

		virtual void BeforeDraw(LPDIRECT3DDEVICE9 device, PCAMERA camera, PNODE rootNode, PGAME game);
		
		virtual void DrawEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{
		}
		
		void Draw(LPDIRECT3DDEVICE9 device, PCAMERA camera, bool flag = false);

		virtual void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{
		}
		
		void Update(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera);

		virtual void ResponseEvent(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{
		}

		 void Response(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera);

		 virtual void Clear()
		 {
			 delete this;
		 }

		 void ClearUp();

		 void RendererToTexture(PNODE node, LPDIRECT3DDEVICE9 device, PCAMERA camera);

		virtual LIFE Life()
		{
			return LIFE_BORN;
		}

	protected:
		CTexture *mTexture, *mSpecialTexture;
		D3DMATERIAL9 mMaterial;
		ID3DXMesh* mMesh;
		std::list<CNodeInterface*> mList;

		LIFE mLife;
		bool mRTTFlag;
	};

}

