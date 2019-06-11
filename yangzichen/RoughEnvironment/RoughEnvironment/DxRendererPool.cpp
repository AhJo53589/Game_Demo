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
			this->ErrorMessage(TEXT("NULL无法创建渲染器"));
			return false;
		}

		this->mGame = game;

		if (!(this->mGame->GethWnd()))
		{
			this->ErrorMessage(TEXT("hWnd丢失"));
			return false;
		}

		//创建Direct3D对象, 该对象用于创建Direct3D设备对象
		this->mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
		if (!(this->mDirect3D))
		{
			this->ErrorMessage(TEXT("创建Direct3D对象失败"));
			return false;
		}

		/*
		typedef struct D3DDISPLAYMODE{
			UNIT         Width;
			UNIT         Height;
			UNIT         RefreshRate;	默认刷新率
			D3DFORMAT   Format;		颜色格式
		};
		*/
		D3DDISPLAYMODE mode;

		//用于取得适配器信息,保存在mode D3DADAPTER_DEFAULT : 当前默认适配器(显卡)
		if (FAILED(this->mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode)))
		{
			this->ErrorMessage(TEXT("获取适配器信息失败"));
			return false;
		}

		/*
		用于储存硬件功能的结构体
		这个结构的内容多到蛋疼
		*/
		D3DCAPS9 caps;

		//检测硬件 D3DADAPTER_DEFAULT : 检测显卡, D3DDEVTYPE_HAL：模式(硬件), cap : 储存结果的结构体
		if (FAILED(this->mDirect3D->GetDeviceCaps( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
		{
			this->ErrorMessage(TEXT("获取适配器硬件能力失败"));
			return false;
		}

		//如果硬件支持顶点处理则使用硬件、否则……
		DWORD processing = 0;
		if (caps.VertexProcessingCaps != 0)
			processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
		else
			processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

		/*
		typedef struct _D3DPRESENT_PARAMETERS_ {
			UINT BackBufferWidth;										后备缓冲的宽度（以像素为单位）
			UINT BackBufferHeight;									后备缓冲的高度（以像素为单位）
			D3DFORMAT BackBufferFormat;						后备缓冲的像素格式
			UINT BackBufferCount;									后备缓冲的数量，通常设为“1”，即只有一个后备表面
			D3DMULTISAMPLE_TYPE MultiSampleType;		全屏抗锯齿的类型
			DWORD MultiSampleQuality;							全屏抗锯齿的质量等级
			D3DSWAPEFFECT SwapEffect;							指定表面在交换链中是如何被交换的，取D3DSWAPEFFECT枚举类型中的一个成员。其中D3DSWAPEFFECT_DISCARD是最有效的
			HWND hDeviceWindow;									与设备相关的窗口句柄，你想在哪个窗口绘制就写那个窗口的句柄
			BOOL Windowed;												BOOL型，设为true则为窗口模式，false则为全屏模式
			BOOL EnableAutoDepthStencil;						设为true，D3D将自动创建深度/模版缓冲
			D3DFORMAT AutoDepthStencilFormat;			深度/模版缓冲的格式
			DWORD Flags;													一些附加特性，设为0或D3DPRESENTFLAG类型的一个成员。下列两个最常用的标志
																						D3DPRESENTFLAG_LOCKABLE_BACKBUFFER——设定后备表面能够被锁定，这会降低应用程序的性能
																						D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL——深度/模版缓冲在调用IDirect3DDevice9::present方法后将被删除
			UINT FullScreen_RefreshRateInHz;					刷新率，设定D3DPRESENT_RATE_DEFAULT使用默认刷新率
			UINT PresentationInterval;								属于D3DPRESENT成员，下列常用标志
																						D3DPRESENT_INTERVAL_ONE——等待随下一次刷新交换
																						D3DPRESENT_INTERVAL_IMMEDIATE——立即交换
																						D3DPRESENT_INTERVAL_DEFAULT——D3D选择交换速度，通常等于刷新率
			MultiSampleType												多重采样
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

		//创建IDirect3DDevice9对象 D3DADAPTER_DEFAULT : 适配器, D3DDEVTYPE_HAL : 设备类型 硬件(D3DDEVTYPE_REF 软件), 窗口句柄, 顶点处理方式, D3DPRESENT_PARAMETERS, D3DDecive
		if (FAILED(this->mDirect3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->mGame->GethWnd(), processing,
			&Params, &(this->mDevice))))
		{
			this->ErrorMessage(TEXT("创建IDirect3DDevice9对象失败"));
			return false;
		}

		//灯光
		this->mDevice->SetRenderState(D3DRS_LIGHTING, this->mLightEnable);
		//
		this->mDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		this->mDevice->SetRenderState(D3DRS_SPECULARENABLE, true);
		
		//Z缓存
		this->mDevice->SetRenderState(D3DRS_ZENABLE, this->mZEnable);
		//模板
		this->mDevice->SetRenderState(D3DRS_STENCILENABLE, this->mStencil);
		//背面三角形的剔除方式
		this->mDevice->SetRenderState(D3DRS_CULLMODE, this->mCULL);

		//设置过滤器 用以提高纹理效果
		this->mDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		this->mDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		//设置mipmap过滤器
		this->mDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

		D3DXMATRIX projection;	//projection : 投影

		//设置投影矩阵 输出矩阵, y轴视角, 宽高比, 近裁面, 远裁面
		D3DXMatrixPerspectiveFovLH(&projection, 45.0f, (float)this->mGame->mWidth/(float)this->mGame->mHeight,
			1.0f, 1000.0f);
	
		//设置矩阵 D3DTS_PROJECTION : 透视矩阵
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
			this->ErrorMessage(TEXT("IDirect3DDevice9对象丢失(CleanBuffers)"), this->mGame->GethWnd());
			return;
		}

		unsigned int buffers = 0;
		if (bColor) 
			buffers |= D3DCLEAR_TARGET;		// 0x 0000 0001 l 目的表面，通常为后备表面
		if (bDepth)
			buffers |= D3DCLEAR_ZBUFFER;	// 0x 0000 0002 l 深度缓冲
		if (bStencil)
			buffers |= D3DCLEAR_STENCIL;	// 0x 0000 0004 | 模版缓冲

		if (FAILED(this->mDevice->Clear( 0, NULL, buffers, mClearColor, 1, 0)))
		{
			this->ErrorMessage(TEXT("清除缓存失败"), this->mGame->GethWnd());
			return;
		}
	}
}