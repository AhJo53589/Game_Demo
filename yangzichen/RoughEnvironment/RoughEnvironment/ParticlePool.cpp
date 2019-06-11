#include "ParticlePool.h"

namespace pool
{
	void  CParticleSystemPool::SetParticleRange(float x, float y, float z, float w, float h, float d)
	{
		this->mSpace.At(x, y, z);

		if (w < 0)
			this->mBoundingBox.x = -w;
		else
			this->mBoundingBox.x = w;

		if (h < 0)
			this->mBoundingBox.y = -h;
		else
			this->mBoundingBox.y = h;

		if (d < 0)
			this->mBoundingBox.z = -d;
		else
			this->mBoundingBox.z = d;
	}

	void  CParticleSystemPool::InitializeEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		if (FAILED(device->CreateVertexBuffer( this->mMaxParticles * sizeof(DxCommon::Vertex_Point),
			D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
			DxCommon::Vertex_Point::FVF, D3DPOOL_DEFAULT, &this->mVertexBuffer, 0)))
		{
			MessageBox(0,0,0,0);
		}
	}

	void  CParticleSystemPool::Initialize(LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{	
		this->ClearUp();
		
		this->BeforeInitialzeEvent(device, camera);
		this->InitializeEvent(device, camera);

		if (!this->mTexture)
			this->mTexture = GetEmptyTexture();

		if (this->mMaxParticles < 0)
			this->mMaxParticles = 1;

		this->mLife = LIFE_LIFE;
	}

	void  CParticleSystemPool::CreateParticles(int num)
	{
		for (int n=0 ; n<num ; ++n)
		{
			if (this->mParticleList.size() >= this->mMaxParticles)
				return;

			this->CreateParticlesEvent();
		}
	}

	void  CParticleSystemPool::UpdateParticle(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		DxCommon::Vertex_Point *v;
		
		if(FAILED(this->mVertexBuffer->Lock(0, this->mParticleList.size() * sizeof(DxCommon::Vertex_Point), (void**)&v, 0))) 
			return;
		int i = 0;
		for (std::list<CParticle>::iterator n=this->mParticleList.begin() ; n!=this->mParticleList.end() ; ++n)
		{
			v[i] = DxCommon::Vertex_Point(n->mPosition.x, n->mPosition.y, n->mPosition.z);
			++i;
		}

		this->mVertexBuffer->Unlock();
	}
	
	void  CParticleSystemPool::Update(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		if (this->mLife == LIFE_BORN)
			this->Initialize(device, camera);
		
		for (std::list<CParticle>::iterator n=this->mParticleList.begin() ; n!=this->mParticleList.end() ; ++n)
		{
			n->mDirection += n->mAcceleration;
			n->mPosition += n->mDirection;

			n->mLifetime += time;
		}
		
		std::list<std::list<CParticle>::iterator> deleteList;

		for (std::list<CParticle>::iterator n=this->mParticleList.begin() ; n!=this->mParticleList.end() ; ++n)
		{
			if (this->mBoundingBoxFlag)
			{
				if (n->mPosition.x < -this->mBoundingBox.x || n->mPosition.x > this->mBoundingBox.x ||
					n->mPosition.y < -this->mBoundingBox.y || n->mPosition.y > this->mBoundingBox.y ||
					n->mPosition.z < -this->mBoundingBox.z || n->mPosition.z > this->mBoundingBox.z)
				{
					deleteList.push_back(n);
					continue;
				}
			}

			if (this->mLifetimeFlag)
			{
				if (n->mLifetime > this->mDeadTime)
				{
					deleteList.push_back(n);
				}
			}
		}

		for (std::list<std::list<CParticle>::iterator>::iterator n=deleteList.begin() ; n!=deleteList.end() ; ++n)
		{
			this->mParticleList.erase(*n);
		}

		this->CreateParticles(this->mCreateParticlesWithTime * time / 1000);
		this->UpdateParticle(time, device, camera);
		this->UpdateEvent(time, device, camera);

	}

	void  CParticleSystemPool::Response(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		this->ResponseEvent(time, input, device, camera);
	}

	void CParticleSystemPool::DrawEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		device->SetFVF(DxCommon::Vertex_Point::FVF);

		device->SetStreamSource(0, this->mVertexBuffer, 0, sizeof(DxCommon::Vertex_Point));

		device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	}

	void  CParticleSystemPool::AfterDrawEvent(LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	}

	void CParticleSystemPool::Draw(LPDIRECT3DDEVICE9 device, PCAMERA camera, bool flag)
	{
		if (this->mLife != LIFE_LIFE)
			return;

		if (flag && this->mRTTFlag)
			return;

		if (this->mVertexBuffer)
		{
			device->BeginScene();

			device->SetVertexShader(NULL);
			device->SetPixelShader(NULL);
			
			device->SetTransform(D3DTS_WORLD, &this->mSpace.GetMatrix());

			device->SetMaterial(&this->mMaterial);

			if (this->mTexture->mTexture)
			{
				device->SetTexture(0, this->mTexture->mTexture);
			}
			else
			{
				device->SetTexture(0, NULL);
			}
			
			device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
			device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
			device->SetRenderState(D3DRS_POINTSIZE, FtoDW(this->mParticleSize));
			device->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(this->mParticleMinSize));
			device->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
			device->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
			device->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));

			this->DrawEvent(device, camera);

			device->DrawPrimitive(D3DPT_POINTLIST, 0, this->mParticleList.size());

			this->AfterDrawEvent(device, camera);

			device->EndScene();
		}
	}

	void  CParticleSystemPool::ClearUp()
	{
		if (this->mVertexBuffer)
			{
				this->mVertexBuffer->Release();
				this->mVertexBuffer = NULL;
			}
	}
}