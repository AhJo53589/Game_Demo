#include "GamePool.h"

namespace pool
{
	CGamePool::CGamePool() : CDeviceErrorInterface(TEXT("GamePool")), mTime(0), moldTime(0),
		mRenderer(NULL), mInput(NULL), mNode(NULL), mCamera(NULL)
	{
	}


	CGamePool::~CGamePool()
	{
		this->ClearUp();
	}

	bool CGamePool::Initialize(HWND hWnd, HINSTANCE hInstance)
	{
		if (!hWnd)
		{
			this->ErrorMessage(TEXT("hWnd获取失败"));
			return false;
		}
		if (!hInstance)
		{
			this->ErrorMessage(TEXT("hInstance获取失败"));
			return false;
		}
		
		this->mhWnd = hWnd;
		this->mhInstance = hInstance;

		addConsole();

		if (!(this->mRenderer))
			this->mRenderer = new CDxRendererPool();

		this->mRenderer->Initialize(this);

		if(!(this->mInput))
			this->mInput = new CDxInputPool();
		this->mInput->Initialize(this);

		if (!(this->mCamera))
			this->mCamera = new CCameraPool();

		if (!(this->mNode))
			this->mNode = new CNodePool();

		return true;
	}

	void CGamePool::Loop()
	{
		this->mInput->UpdateDevice();

		this->moldTime = this->mTime;
		this->mTime = timeGetTime();
		DWORD dtime = this->mTime - this->moldTime;

		if (this->mCamera)
		{

			this->mCamera->Response(dtime, this->mInput);

			this->mCamera->Update(dtime);
		}

		if (this->mNode)
		{
			this->mRenderer->Response(dtime, this->mNode, this->mInput, this->mCamera);

			this->mRenderer->Update(this->mNode, dtime, this->mCamera);

			this->mRenderer->BeforeDraw(this->mNode, this->mCamera, this->mNode);

			this->mRenderer->Draw(this->mNode, this->mCamera);
		}

	}

	void CGamePool::ClearUp()
	{
		if (this->mRenderer)
		{
			delete this->mRenderer;
			this->mRenderer = NULL;
		}

		if (this->mInput)
		{
			delete this->mInput;
			this->mInput = NULL;
		}

		if (this->mCamera)
		{
			delete this->mCamera;
			this->mCamera = NULL;
		}

		if (this->mNode)
		{
			delete this->mNode;
			this->mNode = NULL;
		}

	}

	DWORD CGamePool::GetTimeDelta()
	{
		return this->mTime - this->moldTime;
	}

	void CGamePool::SetNode(PNODE node)
	{
		if (this->mNode)
		{
			delete this->mNode;
			this->mNode = NULL;
		}
		this->mNode = node;
	}

	void CGamePool::SetCamera(PCAMERA camera)
	{
		if (this->mCamera)
		{
			delete this->mCamera;
			this->mCamera = NULL;
		}
		this->mCamera = camera;
	}
}
