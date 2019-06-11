#include "NodePool.h"

namespace pool
{
	CNodePool::CNodePool() : mMesh(NULL), mTexture(NULL), mSpecialTexture(NULL), mLife(LIFE_BORN), mRTTFlag(false)
	{
	}

	CNodePool::~CNodePool()
	{
		this->ClearUp();
	}

	void CNodePool::Initialize(LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		this->ClearUp();

		this->InitializeEvent(device,camera);

		if (!this->mTexture)
			this->mTexture = GetEmptyTexture();
		if (!this->mSpecialTexture)
			this->mSpecialTexture = GetEmptyTexture();

		this->mLife = LIFE_LIFE;
	}

	void CNodePool::Update(DWORD time, LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		if (this->mLife == LIFE_BORN)
			this->Initialize(device, camera);
		
		this->UpdateEvent(time, device, camera);

		for (std::list<CNodeInterface*>::iterator n=this->mList.begin() ; n!=this->mList.end() ; ++n)
		{
			(*n)->Update(time, device, camera);

			(*n)->SetParentSpace(&this->mSpace);
		}
	}

	void CNodePool::Response(DWORD time, PINPUT input, LPDIRECT3DDEVICE9 device, PCAMERA camera)
	{
		this->ResponseEvent(time, input, device, camera);

		for (std::list<CNodeInterface*>::iterator n=this->mList.begin() ; n!=this->mList.end() ; ++n)
		{
			(*n)->Response(time, input, device, camera);
		}
	}

	void CNodePool::BeforeDraw(LPDIRECT3DDEVICE9 device, PCAMERA camera, PNODE rootNode, PGAME game)
	{
		if (this->mLife != LIFE_LIFE)
			return;

		this->BeforeDrawEvent(device, camera, rootNode, game);

		for (std::list<CNodeInterface*>::iterator n=this->mList.begin() ; n!=this->mList.end() ; ++n)
		{
			(*n)->BeforeDraw(device, camera, rootNode, game);
		}
	}

	void CNodePool::Draw(LPDIRECT3DDEVICE9 device, PCAMERA camera, bool flag)
	{		

		if (this->mLife != LIFE_LIFE)
			return;

		for (std::list<CNodeInterface*>::iterator n=this->mList.begin() ; n!=this->mList.end() ; ++n)
		{
			(*n)->Draw(device, camera, flag);
		}

		if (flag && this->mRTTFlag)
			return;

		if (this->mMesh)
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

			if (this->mSpecialTexture->mTexture)
			{
				device->SetTexture(1, this->mSpecialTexture->mTexture);
			}
			else
			{
				device->SetTexture(1, NULL);
			}

			this->DrawEvent(device, camera);
			
			this->mMesh->DrawSubset(0);

			device->EndScene();
		}
	}

	void CNodePool::ClearUp()
	{
		if (this->mMesh)
		{
			this->mMesh->Release();
			this->mMesh = NULL;
		}

		for (std::list<CNodeInterface*>::iterator n=this->mList.begin() ; n!=this->mList.end() ; ++n)
		{
			if(*n)
			{
				delete (*n);
			}
		}

		this->mList.clear();
	}

	 void  CNodePool::RendererToTexture(PNODE node, LPDIRECT3DDEVICE9 device, PCAMERA camera)
	 {

		 LPDIRECT3DSURFACE9 backSurface = NULL;

		 device->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &backSurface);
		 device->SetRenderTarget(0, this->mTexture->mOffScreenSurface);

		 device->Clear(0, NULL, D3DCLEAR_TARGET |  D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

		 device->SetTransform(D3DTS_VIEW, &camera->GetViewMatrix());

		 device->BeginScene();

		 node->Draw(device, camera, true);

		 device->EndScene();

		 device->SetRenderTarget(0, backSurface);

	 }
}