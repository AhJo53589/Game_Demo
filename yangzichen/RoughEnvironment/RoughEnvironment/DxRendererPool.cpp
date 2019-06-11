#include "DxRendererPool.h"

namespace pool
{
	CDxRendererPool::CDxRendererPool()  : CRendererInterface(), CDeviceErrorInterface(TEXT("DxDevicePool")),
		mMSL(eMSL::p_MSL_0), mLightEnable(true), mZEnable(true), mStencil(false), mCULL(D3DCULL_NONE/*D3DCULL_CCW*/), mClearColor(D3DCOLOR_COLORVALUE(0.0f, 0.0f, 1.0f, 1.0f)),
		mDirect3D(NULL), mDevice(NULL)
	{
	}

	CDxRendererPool::~CDxRendererPool()
	{
		this->ClearUp();
	}

	bool CDxRendererPool::Initialize(PGAME game)
	{
		this->ClearUp();

		if (!game)
		{
			this->ErrorMessage(TEXT("NULL�޷�������Ⱦ��"));
			return false;
		}

		this->mGame = game;

		if (!(this->mGame->GethWnd()))
		{
			this->ErrorMessage(TEXT("hWnd��ʧ"));
			return false;
		}

		//����Direct3D����, �ö������ڴ���Direct3D�豸����
		this->mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (!(this->mDirect3D))
		{
			this->ErrorMessage(TEXT("����Direct3D����ʧ��"));
			return false;
		}

		/*
		typedef struct D3DDISPLAYMODE{
			UNIT         Width;
			UNIT         Height;
			UNIT         RefreshRate;	Ĭ��ˢ����
			D3DFORMAT   Format;		��ɫ��ʽ
		};
		*/
		D3DDISPLAYMODE mode;

		//����ȡ����������Ϣ,������mode D3DADAPTER_DEFAULT : ��ǰĬ��������(�Կ�)
		if (FAILED(this->mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
		{
			this->ErrorMessage(TEXT("��ȡ��������Ϣʧ��"));
			return false;
		}

		/*
		���ڴ���Ӳ�����ܵĽṹ��
		����ṹ�����ݶൽ����
		*/
		D3DCAPS9 caps;

		//���Ӳ�� D3DADAPTER_DEFAULT : ����Կ�, D3DDEVTYPE_HAL��ģʽ(Ӳ��), cap : �������Ľṹ��
		if (FAILED(this->mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		{
			this->ErrorMessage(TEXT("��ȡ������Ӳ������ʧ��"));
			return false;
		}

		//���Ӳ��֧�ֶ��㴦����ʹ��Ӳ�������򡭡�
		DWORD processing = 0;
		if (caps.VertexProcessingCaps != 0)
			processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
		else
			processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		/*
		typedef struct _D3DPRESENT_PARAMETERS_ {
			UINT BackBufferWidth;										�󱸻���Ŀ�ȣ�������Ϊ��λ��
			UINT BackBufferHeight;									�󱸻���ĸ߶ȣ�������Ϊ��λ��
			D3DFORMAT BackBufferFormat;						�󱸻�������ظ�ʽ
			UINT BackBufferCount;									�󱸻����������ͨ����Ϊ��1������ֻ��һ���󱸱���
			D3DMULTISAMPLE_TYPE MultiSampleType;		ȫ������ݵ�����
			DWORD MultiSampleQuality;							ȫ������ݵ������ȼ�
			D3DSWAPEFFECT SwapEffect;							ָ�������ڽ�����������α������ģ�ȡD3DSWAPEFFECTö�������е�һ����Ա������D3DSWAPEFFECT_DISCARD������Ч��
			HWND hDeviceWindow;									���豸��صĴ��ھ�����������ĸ����ڻ��ƾ�д�Ǹ����ڵľ��
			BOOL Windowed;												BOOL�ͣ���Ϊtrue��Ϊ����ģʽ��false��Ϊȫ��ģʽ
			BOOL EnableAutoDepthStencil;						��Ϊtrue��D3D���Զ��������/ģ�滺��
			D3DFORMAT AutoDepthStencilFormat;			���/ģ�滺��ĸ�ʽ
			DWORD Flags;													һЩ�������ԣ���Ϊ0��D3DPRESENTFLAG���͵�һ����Ա������������õı�־
																						D3DPRESENTFLAG_LOCKABLE_BACKBUFFER�����趨�󱸱����ܹ�����������ή��Ӧ�ó��������
																						D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL�������/ģ�滺���ڵ���IDirect3DDevice9::present�����󽫱�ɾ��
			UINT FullScreen_RefreshRateInHz;					ˢ���ʣ��趨D3DPRESENT_RATE_DEFAULTʹ��Ĭ��ˢ����
			UINT PresentationInterval;								����D3DPRESENT��Ա�����г��ñ�־
																						D3DPRESENT_INTERVAL_ONE�����ȴ�����һ��ˢ�½���
																						D3DPRESENT_INTERVAL_IMMEDIATE������������
																						D3DPRESENT_INTERVAL_DEFAULT����D3Dѡ�񽻻��ٶȣ�ͨ������ˢ����
			MultiSampleType												���ز���
		} 
		*/
		D3DPRESENT_PARAMETERS Params;
		ZeroMemory( &Params, sizeof(Params));

		if (this->mGame->mFullscreen)
		{
			Params.FullScreen_RefreshRateInHz = mode.RefreshRate;	
			Params.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
		}
		else
			Params.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		
		Params.Windowed = !(this->mGame->mFullscreen);
		Params.BackBufferWidth = this->mGame->mBackBufferWidth;										
		Params.BackBufferHeight =  this->mGame->mBackBufferHeight;										
		Params.hDeviceWindow = this->mGame->GethWnd();				
		Params.SwapEffect = D3DSWAPEFFECT_DISCARD;		
		Params.BackBufferFormat = mode.Format;					
		Params.BackBufferCount = 1;										
		Params.EnableAutoDepthStencil = TRUE;						
		Params.AutoDepthStencilFormat = D3DFMT_D16;		
		Params.MultiSampleType = this->GetMultiSampleLevel(mode.Format);

		//����IDirect3DDevice9���� D3DADAPTER_DEFAULT : ������, D3DDEVTYPE_HAL : �豸���� Ӳ��(D3DDEVTYPE_REF ���), ���ھ��, ���㴦��ʽ, D3DPRESENT_PARAMETERS, D3DDecive
		if (FAILED(this->mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->mGame->GethWnd(), processing,
			&Params, &(this->mDevice))))
		{
			this->ErrorMessage(TEXT("����IDirect3DDevice9����ʧ��"));
			return false;
		}

		//�ƹ�
		this->mDevice->SetRenderState(D3DRS_LIGHTING, this->mLightEnable);
		//
		this->mDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		this->mDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		
		//Z����
		this->mDevice->SetRenderState(D3DRS_ZENABLE, this->mZEnable);
		//ģ��
		this->mDevice->SetRenderState(D3DRS_STENCILENABLE, this->mStencil);
		//���������ε��޳���ʽ
		this->mDevice->SetRenderState(D3DRS_CULLMODE, this->mCULL);

		//���ù����� �����������Ч��
		this->mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		this->mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		//����mipmap������
		this->mDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		D3DXMATRIX projection;	//projection : ͶӰ

		//����ͶӰ���� �������, y���ӽ�, ��߱�, ������, Զ����
		D3DXMatrixPerspectiveFovLH(&projection, 45.0f, (float)this->mGame->mWidth/(float)this->mGame->mHeight,
			1.0f, 1000.0f);
	
		//���þ��� D3DTS_PROJECTION : ͸�Ӿ���
		this->mDevice->SetTransform(D3DTS_PROJECTION, &projection);


		InitResManager(this->mDevice);
		return true;
	}

	D3DMULTISAMPLE_TYPE CDxRendererPool::GetMultiSampleLevel(D3DFORMAT format)
	{
		D3DMULTISAMPLE_TYPE temp = D3DMULTISAMPLE_NONE;

		if(this->mDirect3D)
		{
			switch(this->mMSL)
			{
			case eMSL::p_MSL_0:
				temp = D3DMULTISAMPLE_NONE;
            break;

			case eMSL::p_MSL_2:
				if(this->mDirect3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, 
					!(this->mGame->mFullscreen), D3DMULTISAMPLE_2_SAMPLES, NULL) == D3D_OK) 
					temp = D3DMULTISAMPLE_2_SAMPLES;
				break;

			case eMSL::p_MSL_4:
				if(this->mDirect3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,	D3DDEVTYPE_HAL, format, 
					!(this->mGame->mFullscreen), D3DMULTISAMPLE_2_SAMPLES,NULL) == D3D_OK) 
					temp = D3DMULTISAMPLE_4_SAMPLES;
				break;

			case eMSL::p_MSL_8:
				if(this->mDirect3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, 
					!(this->mGame->mFullscreen), D3DMULTISAMPLE_8_SAMPLES, NULL) == D3D_OK) 
					temp = D3DMULTISAMPLE_8_SAMPLES;
				break;

			case eMSL::p_MSL_16:
				if(this->mDirect3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, format, 
					!(this->mGame->mFullscreen), D3DMULTISAMPLE_16_SAMPLES, NULL) == D3D_OK) 
					temp = D3DMULTISAMPLE_16_SAMPLES;
				break;

			default:
				break;
			}
		}

	return temp;
	}

	void CDxRendererPool::Response(DWORD time, PNODE node, PINPUT input, PCAMERA camera)
	{

		if (node)
		{
			node->Response(time, input, this->mDevice, camera);
		}

	}

	void CDxRendererPool::Update(PNODE node, DWORD time, PCAMERA camera)
	{
		if (node)
		{
			node->Update(time, this->mDevice, camera);
		}
	}

	void CDxRendererPool::BeforeDraw(PNODE node, PCAMERA camera, PNODE rootNode)
	{
		D3DXMATRIX projection;
		
		D3DXMatrixPerspectiveFovLH(&projection, 45.0f, (float)this->mGame->mWidth/(float)this->mGame->mHeight,
			1.0f, 1000.0f);
	
		this->mDevice->SetTransform(D3DTS_PROJECTION, &projection);

		if (node)
		{
			node->BeforeDraw(this->mDevice, camera, rootNode, this->mGame);
		}
	}

	void CDxRendererPool::Draw(PNODE node, PCAMERA camera)
	{
		this->CleanBuffers(true, true, false);

		this->mDevice->SetTransform(D3DTS_VIEW, &camera->GetViewMatrix());

		if (node)
		{
			node->Draw(this->mDevice, camera);
		}

		this->mDevice->Present(0, 0, 0, 0);

	}

	void CDxRendererPool::ClearUp()
	{
		if (this->mDevice)
		{
			this->mDevice->Release();
			this->mDevice = NULL;
		}

		if (this->mDirect3D)
		{
			this->mDirect3D->Release();
			this->mDirect3D = NULL;
		}
	}

	void CDxRendererPool::CleanBuffers(bool bColor, bool bDepth, bool bStencil)
	{
		if (!(this->mDevice))
		{
			this->ErrorMessage(TEXT("IDirect3DDevice9����ʧ(CleanBuffers)"), this->mGame->GethWnd());
			return;
		}

		unsigned int buffers = 0;
		if (bColor) 
			buffers |= D3DCLEAR_TARGET;		// 0x 0000 0001 l Ŀ�ı��棬ͨ��Ϊ�󱸱���
		if (bDepth)
			buffers |= D3DCLEAR_ZBUFFER;	// 0x 0000 0002 l ��Ȼ���
		if (bStencil)
			buffers |= D3DCLEAR_STENCIL;	// 0x 0000 0004 | ģ�滺��

		if (FAILED(this->mDevice->Clear( 0, NULL, buffers, mClearColor, 1, 0)))
		{
			this->ErrorMessage(TEXT("�������ʧ��"), this->mGame->GethWnd());
			return;
		}
	}
}