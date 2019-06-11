#include "Game2drpg.h"
#include "Define2DRPG.h"

Game2drpg *game = NULL;
LPDIRECT3DDEVICE9 d3dDevice = NULL;

Game2drpg::Game2drpg(HINSTANCE hInstance, TCHAR *wndCaption) : mhInstance(hInstance), mWndCaption(wndCaption), mhWnd(NULL), mDirect3D(NULL), mInput(NULL), mCamera(NULL), mRes(NULL)
{
	ZeroMemory(&this->mParams, sizeof(this->mParams));
}

Game2drpg::~Game2drpg() 
{
	Delete(this->mUIManager);
	Delete(this->mRes);
	Delete(this->mPlayer);
	Delete(this->mCamera);
	Delete(this->mUICamera);
	Delete(this->mWorldMap);
	Delete(this->mInput);

	ReleaseCOM(this->mDirect3D);
	ReleaseCOM(d3dDevice);
}

Game2drpg &Game2drpg::create()
{
	this->initWindow();
	this->initDirect3D();

	return *this;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if(game)
		return game->msgProc(msg, wParam, lParam);
	else
		return DefWindowProc(hwnd, msg, wParam, lParam);
}

#define WINDOWSTYLE (WS_OVERLAPPED     | \
	WS_CAPTION        | \
	WS_SYSMENU        | \
	WS_MINIMIZEBOX)

void Game2drpg::initWindow()
{
	WNDCLASS wndClass;
	//从这个窗口类派生的窗口具有的风格
	wndClass.style = 0;
	//窗口处理函数的指针
	wndClass.lpfnWndProc = WndProc;
	//指定紧跟在窗口类结构后的附加字节数
	wndClass.cbClsExtra = 0;
	//指定紧跟在窗口事例后的附加字节数
	wndClass.cbWndExtra = 0;
	//本模块的事例句柄
	wndClass.hInstance = this->mhInstance;
	//图标的句柄
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);;
	//光标的句柄
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//背景画刷的句柄
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//指向菜单的指针
	wndClass.lpszMenuName = NULL;
	//指向类名称的指针
	wndClass.lpszClassName = TEXT("D3DGameWindowClass");

	if(!RegisterClass(&wndClass))
	{
		MessageBox(0, "RegisterClass FAILED", 0, 0);
		PostQuitMessage(0);
	}

	RECT rc;
	SetRect(&rc, 0, 0, RESOLUTIONW, RESOLUTIONH);
	AdjustWindowRect(&rc, WINDOWSTYLE, false);

	this->mhWnd = CreateWindow(TEXT("D3DGameWindowClass"), this->mWndCaption, WINDOWSTYLE, 0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, NULL, this->mhInstance, 0);

	if( !mhWnd )
	{
		MessageBox(0, "CreateWindow FAILED", 0, 0);
		PostQuitMessage(0);
	}

	ShowWindow(this->mhWnd, SW_SHOW);
	UpdateWindow(this->mhWnd);

	// 分配一个控制台程序  
	//AllocConsole();
	//freopen("CONOUT$","w+t",stdout);
	//freopen("CONIN$","r+t",stdin);
}

void Game2drpg::initDirect3D()
{
	//创建Direct3D对象, 该对象用于创建Direct3D设备对象
	this->mDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (!(this->mDirect3D))
	{
		MessageBox(0, "Direct3DCreate9 FAILED", 0, 0);
		PostQuitMessage(0);
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
	HR(this->mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode));

	/*
	用于储存硬件功能的结构体
	*/
	D3DCAPS9 caps;

	//检测硬件 D3DADAPTER_DEFAULT : 检测显卡, D3DDEVTYPE_HAL：模式(硬件), cap : 储存结果的结构体
	HR(this->mDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));

	//如果硬件支持顶点处理则使用硬件
	DWORD processing = 0;
	if (caps.VertexProcessingCaps != 0)
		processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	/*
	typedef struct _D3DPRESENT_PARAMETERS_ {
	UINT BackBufferWidth; 后备缓冲的宽度（以像素为单位）
	UINT BackBufferHeight; 后备缓冲的高度（以像素为单位）
	D3DFORMAT BackBufferFormat; 后备缓冲的像素格式
	UINT BackBufferCount; 后备缓冲的数量，通常设为“1”，即只有一个后备表面
	D3DMULTISAMPLE_TYPE MultiSampleType; 全屏抗锯齿的类型
	DWORD MultiSampleQuality; 全屏抗锯齿的质量等级
	D3DSWAPEFFECT SwapEffect; 指定表面在交换链中是如何被交换的，取D3DSWAPEFFECT枚举类型中的一个成员。其中D3DSWAPEFFECT_DISCARD是最有效的
	HWND hDeviceWindow; 与设备相关的窗口句柄，你想在哪个窗口绘制就写那个窗口的句柄
	BOOL Windowed; BOOL型，设为true则为窗口模式，false则为全屏模式
	BOOL EnableAutoDepthStencil; 设为true，D3D将自动创建深度/模版缓冲
	D3DFORMAT AutoDepthStencilFormat; 深度/模版缓冲的格式
	DWORD Flags; 一些附加特性，设为0或D3DPRESENTFLAG类型的一个成员。下列两个最常用的标志
				  D3DPRESENTFLAG_LOCKABLE_BACKBUFFER——设定后备表面能够被锁定，这会降低应用程序的性能
				  D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL——深度/模版缓冲在调用IDirect3DDevice9::present方法后将被删除
	UINT FullScreen_RefreshRateInHz; 刷新率，设定D3DPRESENT_RATE_DEFAULT使用默认刷新率
	UINT PresentationInterval; 属于D3DPRESENT成员，下列常用标志
								 D3DPRESENT_INTERVAL_ONE——等待随下一次刷新交换
								 D3DPRESENT_INTERVAL_IMMEDIATE——立即交换
								 D3DPRESENT_INTERVAL_DEFAULT——D3D选择交换速度，通常等于刷新率
	MultiSampleType 多重采样
	} 
	*/

	this->mParams.BackBufferWidth            = 800; 
	this->mParams.BackBufferHeight           = 600;
	this->mParams.BackBufferFormat           = D3DFMT_UNKNOWN;
	this->mParams.BackBufferCount            = 1;
	this->mParams.MultiSampleType            = D3DMULTISAMPLE_NONE;
	this->mParams.MultiSampleQuality         = 0;
	this->mParams.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	this->mParams.hDeviceWindow              = this->mhWnd;
	this->mParams.Windowed                   = true;
	this->mParams.EnableAutoDepthStencil     = true; 
	this->mParams.AutoDepthStencilFormat     = D3DFMT_D24S8;
	this->mParams.Flags                      = 0;
	this->mParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	this->mParams.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;

	//创建IDirect3DDevice9对象 D3DADAPTER_DEFAULT : 适配器, D3DDEVTYPE_HAL : 设备类型 硬件(D3DDEVTYPE_REF 软件), 窗口句柄, 顶点处理方式, D3DPRESENT_PARAMETERS, D3DDecive
	HR(this->mDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->mhWnd, processing, &this->mParams, &d3dDevice));

	//关闭灯光
	d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	//设置过滤器 用以提高纹理效果
	d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//设置mipmap过滤器
	d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//开启透明
	d3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	d3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	d3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

int Game2drpg::playing()
{
	MSG  msg;
    msg.message = WM_NULL;

	DWORD oldtime = timeGetTime(), nowtime;
	float dtime;

	while(msg.message != WM_QUIT)
	{
		if(PeekMessage( &msg, 0, 0, 0, PM_REMOVE ))
		{
            TranslateMessage( &msg );
            DispatchMessage( &msg );
		}
		else
        {	
			if(!this->isDeviceLost())
			{
				nowtime = timeGetTime();
				dtime = (nowtime - oldtime) / 1000.0f;
				
				this->update(dtime);
				this->draw();
				
				oldtime = nowtime;
			}
        }
    }

	return (int)msg.wParam;
}

LRESULT Game2drpg::msgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_CLOSE:
		DestroyWindow(this->mhWnd);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(this->mhWnd, msg, wParam, lParam);
}

bool Game2drpg::isDeviceLost()
{	//获取d3dDevice状态
	HRESULT hr = d3dDevice->TestCooperativeLevel();

	switch(hr)
	{
	case D3DERR_DEVICELOST: // 如果设备丢失则Sleep一段时间，并在下次循环中尝试唤醒
		Sleep(20);
		return true;

	case D3DERR_DRIVERINTERNALERROR: // 发生错误
		MessageBox(0, "d3dDevice发生错误", 0, 0);
		PostQuitMessage(0);
		return true;

	case D3DERR_DEVICENOTRESET: // 如果设备丢失但是能够恢复
		this->onLostDevice();
		HR(d3dDevice->Reset(&this->mParams));
		this->onResetDevice();
		return false;

	default:
		return false;
	}
}

void Game2drpg::init()
{
	this->mInput = new DInput(this);

#if !defined(TEST3D)
	Camera2drpg *tmpCamera = new Camera2drpg();
	tmpCamera->init(RESOLUTIONW, RESOLUTIONH, 1.0f, 1000.0f);
	tmpCamera->at(0 , 0);
	this->mCamera = tmpCamera;
#else
	Camera3d *tmpCamera = new Camera3dUse();
	tmpCamera->init(45.0f, static_cast<float>(RESOLUTIONW)/static_cast<float>(RESOLUTIONH), 1.0f, 1000.0f);
	tmpCamera->setCameraFree(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(0, 0, -500));
	this->mCamera = tmpCamera;
#endif

	this->mUICamera = new Camera2d();
	this->mUICamera->init(RESOLUTIONW, RESOLUTIONH, 1.0f, 1000.0f);
	this->mUICamera->at(RESOLUTIONW * 0.5f, RESOLUTIONH * -0.5f);


	this->mRes = new Res();

	this->mWorldMap = new WorldMap();
	this->mWorldMap->init();

	this->mPlayer = new Player();

	this->mInput->addListener(this->mPlayer);
	this->mPlayer->addListener(dynamic_cast<MovementEventListener *>(this->mCamera));
	this->mCamera->addListener(this->mWorldMap);
	
	this->mUIManager = new UIManager();
	this->mInput->addListener(this->mUIManager);
}

void Game2drpg::update(float dt)
{
	this->mInput->updateDevice(dt);

	this->mPlayer->update(dt);
	this->mWorldMap->update(dt);

	this->mUIManager->update(dt);
}

void Game2drpg::draw()
{
	HR(d3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_COLORVALUE(0.2f, 0.2f, 0.9f, 1.0f), 1.0f, 0));

	this->mWorldMap->draw();
	this->mPlayer->draw();

	this->mUIManager->draw();

	HR(d3dDevice->Present(0, 0, 0, 0));
}