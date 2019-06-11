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
	//����������������Ĵ��ھ��еķ��
	wndClass.style = 0;
	//���ڴ�������ָ��
	wndClass.lpfnWndProc = WndProc;
	//ָ�������ڴ�����ṹ��ĸ����ֽ���
	wndClass.cbClsExtra = 0;
	//ָ�������ڴ���������ĸ����ֽ���
	wndClass.cbWndExtra = 0;
	//��ģ����������
	wndClass.hInstance = this->mhInstance;
	//ͼ��ľ��
	wndClass.hIcon = LoadIcon(0, IDI_APPLICATION);;
	//���ľ��
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	//������ˢ�ľ��
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	//ָ��˵���ָ��
	wndClass.lpszMenuName = NULL;
	//ָ�������Ƶ�ָ��
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

	// ����һ������̨����  
	//AllocConsole();
	//freopen("CONOUT$","w+t",stdout);
	//freopen("CONIN$","r+t",stdin);
}

void Game2drpg::initDirect3D()
{
	//����Direct3D����, �ö������ڴ���Direct3D�豸����
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
	UNIT         RefreshRate;	Ĭ��ˢ����
	D3DFORMAT   Format;		��ɫ��ʽ
	};
	*/
	D3DDISPLAYMODE mode;

	//����ȡ����������Ϣ,������mode D3DADAPTER_DEFAULT : ��ǰĬ��������(�Կ�)
	HR(this->mDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &mode));

	/*
	���ڴ���Ӳ�����ܵĽṹ��
	*/
	D3DCAPS9 caps;

	//���Ӳ�� D3DADAPTER_DEFAULT : ����Կ�, D3DDEVTYPE_HAL��ģʽ(Ӳ��), cap : �������Ľṹ��
	HR(this->mDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps));

	//���Ӳ��֧�ֶ��㴦����ʹ��Ӳ��
	DWORD processing = 0;
	if (caps.VertexProcessingCaps != 0)
		processing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
	else
		processing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	/*
	typedef struct _D3DPRESENT_PARAMETERS_ {
	UINT BackBufferWidth; �󱸻���Ŀ�ȣ�������Ϊ��λ��
	UINT BackBufferHeight; �󱸻���ĸ߶ȣ�������Ϊ��λ��
	D3DFORMAT BackBufferFormat; �󱸻�������ظ�ʽ
	UINT BackBufferCount; �󱸻����������ͨ����Ϊ��1������ֻ��һ���󱸱���
	D3DMULTISAMPLE_TYPE MultiSampleType; ȫ������ݵ�����
	DWORD MultiSampleQuality; ȫ������ݵ������ȼ�
	D3DSWAPEFFECT SwapEffect; ָ�������ڽ�����������α������ģ�ȡD3DSWAPEFFECTö�������е�һ����Ա������D3DSWAPEFFECT_DISCARD������Ч��
	HWND hDeviceWindow; ���豸��صĴ��ھ�����������ĸ����ڻ��ƾ�д�Ǹ����ڵľ��
	BOOL Windowed; BOOL�ͣ���Ϊtrue��Ϊ����ģʽ��false��Ϊȫ��ģʽ
	BOOL EnableAutoDepthStencil; ��Ϊtrue��D3D���Զ��������/ģ�滺��
	D3DFORMAT AutoDepthStencilFormat; ���/ģ�滺��ĸ�ʽ
	DWORD Flags; һЩ�������ԣ���Ϊ0��D3DPRESENTFLAG���͵�һ����Ա������������õı�־
				  D3DPRESENTFLAG_LOCKABLE_BACKBUFFER�����趨�󱸱����ܹ�����������ή��Ӧ�ó��������
				  D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL�������/ģ�滺���ڵ���IDirect3DDevice9::present�����󽫱�ɾ��
	UINT FullScreen_RefreshRateInHz; ˢ���ʣ��趨D3DPRESENT_RATE_DEFAULTʹ��Ĭ��ˢ����
	UINT PresentationInterval; ����D3DPRESENT��Ա�����г��ñ�־
								 D3DPRESENT_INTERVAL_ONE�����ȴ�����һ��ˢ�½���
								 D3DPRESENT_INTERVAL_IMMEDIATE������������
								 D3DPRESENT_INTERVAL_DEFAULT����D3Dѡ�񽻻��ٶȣ�ͨ������ˢ����
	MultiSampleType ���ز���
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

	//����IDirect3DDevice9���� D3DADAPTER_DEFAULT : ������, D3DDEVTYPE_HAL : �豸���� Ӳ��(D3DDEVTYPE_REF ���), ���ھ��, ���㴦��ʽ, D3DPRESENT_PARAMETERS, D3DDecive
	HR(this->mDirect3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, this->mhWnd, processing, &this->mParams, &d3dDevice));

	//�رյƹ�
	d3dDevice->SetRenderState(D3DRS_LIGHTING, false);
	//���ù����� �����������Ч��
	d3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	d3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	//����mipmap������
	d3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

	//����͸��
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
{	//��ȡd3dDevice״̬
	HRESULT hr = d3dDevice->TestCooperativeLevel();

	switch(hr)
	{
	case D3DERR_DEVICELOST: // ����豸��ʧ��Sleepһ��ʱ�䣬�����´�ѭ���г��Ի���
		Sleep(20);
		return true;

	case D3DERR_DRIVERINTERNALERROR: // ��������
		MessageBox(0, "d3dDevice��������", 0, 0);
		PostQuitMessage(0);
		return true;

	case D3DERR_DEVICENOTRESET: // ����豸��ʧ�����ܹ��ָ�
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