#pragma once

#include "define.h"
#include "NodeInterface.h"
#include "CameraInterface.h"
#include "SpaceState.h"
#include "Texture.h"
#include "ResManager.h"

namespace pool
{
	class CParticle
	{
	public:
		CParticle() : mPosition(0.0f, 0.0f, 0.0f), mDirection(0.0f ,0.0f, 0.0f), mAcceleration(0.0f ,0.0f, 0.0f),
			mLifetime(0)
		{
		}
		~CParticle()
		{
		}

		pool::DxCommon::V3 mPosition;
		pool::DxCommon::V3 mDirection;
		pool::DxCommon::V3 mAcceleration;

		DWORD mLifetime;
	};
	
	class CParticleSystemPool : public CNodeInterface
	{
	public:
		CParticleSystemPool () : mVertexBuffer(NULL), mLife(LIFE_BORN), mMaxParticles(1) , mRTTFlag(false),
			mBoundingBoxFlag(true), mLifetimeFlag(false), mDeadTime(0), mCreateParticlesWithTime(0),
			mParticleSize(1), mParticleMinSize(1)
		{
		}
		~CParticleSystemPool ()
		{
			this->ClearUp();
		}

		void SetParticleRange(float x, float y, float z, float w, float h, float d);

		virtual void BeforeInitialzeEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{
		}

		virtual void InitializeEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera);

		void Initialize(LPDIRECT3DDEVICE9 device, PCAMERA camera);

		virtual void BeforeDraw(LPDIRECT3DDEVICE9 device, PCAMERA camera, PNODE rootNode, PGAME game)
		{
		}
		
		virtual void DrawEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera);

		virtual void AfterDrawEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera);
		
		void Draw(LPDIRECT3DDEVICE9 device, PCAMERA camera, bool flag = false);

		virtual void UpdateEvent(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{
		}

		virtual void UpdateParticle(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera);
		
		void Update(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera);

		virtual void ResponseEvent(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera)
		{
		}

		 void Response(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera);

		 virtual void CreateParticlesEvent()
		 {
		 }

		  void CreateParticles(int num);

		 virtual void Clear()
		 {
			 delete this;
		 }

		void ClearUp();

		virtual LIFE Life()
		{
			return LIFE_BORN;
		}

		static unsigned long FtoDW(float val) 
		{ 
			return *((unsigned long*)&val);
		}

		int mMaxParticles;
		DWORD mDeadTime;
		
		bool mBoundingBoxFlag;
		bool mLifetimeFlag;

		int mCreateParticlesWithTime;

	protected:
		D3DXVECTOR3 mBoundingBox;
		IDirect3DVertexBuffer9* mVertexBuffer;

		CTexture *mTexture;
		D3DMATERIAL9 mMaterial;

		std::list<CParticle> mParticleList;

		float mParticleSize,mParticleMinSize;

		LIFE mLife;
		bool mRTTFlag;
	};

}

