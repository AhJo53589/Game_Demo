//==========================================================================================//
//																							//
//									CI-CLUB	Library											//
//																							//
//										ver 2.0												//
//																							//
//						  Game Relation Library　For Windows                                //
//							 ( DX Higher Rank Function )									//
//																							//
//								   2008.05.23  murasho										//
//																							//
//==========================================================================================//

#pragma			once
#pragma			comment(lib,"winmm.lib")
#pragma			comment(lib,"d3dxof.lib")
#pragma			comment(lib,"dxguid.lib")
#pragma			comment(lib,"d3d9.lib")
#pragma			comment(lib,"d3dx9.lib")
#pragma			comment(lib,"dDraw.lib")
#pragma			comment(lib,"dInput8.lib")

#include		<windows.h>
#include		<math.h>
#include		<stdio.h>
#include		<TCHAR.h>

#include		"Ci-Lib.H"



//======================================================================================//
//																						//
//		DirectX	(Draw&Input) Function	Relation										//
//																						//
//======================================================================================//

//----------------------------------------------------------------------------------//
//	★	Define
//
//----------------------------------------------------------------------------------//
#define		DX_ERR_CreateD3D				1				//		DX　ERROR　CODE		
#define		DX_ERR_CreateD3DDevWin			2
#define		DX_ERR_CreateD3DDevFull			3
#define		DX_ERR_CreateSprite				4
#define		DX_ERR_CreateDInput				5
#define		DX_ERR_CreateDIDev				6
#define		DX_ERR_SetupDI					7

#define		FONT_MAX						2				// 登録フォント枚数
#define		SYS_FONT_SIZE					16              // TEXT文字サイズ
#define		SYS_FONT_TYPE					0


//----------------------------------------------------------------------------------//
//	★	環境 グローバル宣言
//
//----------------------------------------------------------------------------------//

//	パッド情報（フラグ管理する）
struct {
	BOOL		Info;
	u_short		Now;
	u_short		Old;
	u_short		Trg;
	u_short		Pul;
} Pad[4];




//	キー情報（フラグ管理する）
struct {
	u_long		Now;
	u_long		Old;
	u_long		Trg;
	u_long		Pul;
} Key;

// フルキー情報（フラグ管理）
_KEY	FullKEY;

//	Graphics関連のグローバル宣言
LPDIRECT3D9						lpD3D;
LPDIRECT3DDEVICE9				lpD3DDev;
LPD3DXSPRITE					lpSprite;
LPD3DXFONT						lpFont[FONT_MAX];

LPDIRECTDRAW					lpDDraw;			//ダイレクトドロー
LPDIRECTDRAWSURFACE				lpDDSPrim;			//プライマリーサーフェース
LPDIRECTDRAWSURFACE				lpDDSBack;			//バックサーフェース


LPDIRECT3DVERTEXBUFFER9			lpVertexBuffer;


//	Input関連のグローバル宣言
LPDIRECTINPUT8					lpDInput;			//DirectInputオブジェクト
LPDIRECTINPUTDEVICE8			lpDIDev[eIDEVNUM];	//DirectInputDeviceオブジェクト
int								cntIDev;			//DIDevice（ゲームパッドの数）
int								nBtn[eIDEVNUM];		//ゲームパッドのボタン数

// フルスクリ－ンスイッチ
int								FullWinSW;
//----------------------------------------------------------------------------------//
//	★	プロトタイプ宣言
//
//----------------------------------------------------------------------------------//

//Input
HRESULT		CreateDInput(HWND hWnd,LPDIRECTINPUT8 *pDInput);
HRESULT		CreateDIDev(HWND hWnd,LPDIRECTINPUT8 lpDInput);
HRESULT		SetupDI(HWND hWnd);
BOOL		CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext);
BOOL		CALLBACK EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpInst,VOID* pRef);
void		ReleaseDI(LPDIRECTINPUT8 *pDInput,LPDIRECTINPUTDEVICE8 *pDIDev);

//Graphics
HRESULT		CreateD3D(LPDIRECT3D9 *pD3D);
HRESULT		CreateD3DDevWin(HWND hWnd,LPDIRECT3D9 lpD3D,LPDIRECT3DDEVICE9 *pD3DDev);
HRESULT		CreateD3DDevFull(HWND hWnd,LPDIRECT3D9 lpD3D,LPDIRECT3DDEVICE9 *pD3DDev);
HRESULT		CreateSprite(LPDIRECT3DDEVICE9 lpD3DDev,LPD3DXSPRITE *pSprite);
//HRESULT		CreateSF(LPDIRECTDRAWSURFACE *inSF1,LPDIRECTDRAWSURFACE *inSF2,int inW,int inH,HWND hWnd);

HRESULT		CreateVB(LPDIRECT3DDEVICE9 lpD3DDev, LPDIRECT3DVERTEXBUFFER9* pVertexBuffer);

HRESULT		CreateFont(LPDIRECT3DDEVICE9 lpD3DDev,LPD3DXFONT *pFont);
HRESULT		LoadTexture(LPCTSTR fName,TEX9 *pTex);
void		ReleaseD3D(LPDIRECT3D9 *pD3D,LPDIRECT3DDEVICE9 *pD3DDev);
void		ReleaseSprite(LPD3DXSPRITE *pSprite);

void		ReleaseVB(LPDIRECT3DVERTEXBUFFER9 *pVB);
void		ReleaseFont(LPD3DXFONT *pFont);
void		ReleaseTexture(TEX9 *pTex);
void		BeginDraw(void);
void		EndDraw(void);

//Env
HRESULT		InitEnv(HWND hWnd);
void		CloseEnv(void);

void		DrawTexture(TEX9,POINT *,RECT *,float ,POINT *,float,float,GsCOLOR);
void		DrawTextureR(TEX9,POINT *,RECT *,float ,POINT *,float,float,GsCOLOR);
void		DrawTextOut(int x,int y,char *str,GsCOLOR,int no,int size);

HRESULT		GetPadInfo(void);
short		GetPadNow(int id);
short		GetPadTrg(int id);
short		GetPadPul(int id);

HRESULT		CreateDMusic(HWND hWnd);
void		ReleaseDMusic(void);

void		SetupLight(void);

//―――――――――――――――――――――――――――――――――――――――――//
//	DirectInputの生成
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateDInput(HWND hWnd,LPDIRECTINPUT8 *pDInput)
{
//	if(FAILED(DirectInput8Create((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),
	if(FAILED(DirectInput8Create((HINSTANCE)GetWindowLong(hWnd,GWL_HINSTANCE),
			DIRECTINPUT_VERSION,IID_IDirectInput8,(void**)pDInput,NULL))){
		return E_FAIL;
	}
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  DirectInputデバイスの生成
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateDIDev(HWND hWnd, LPDIRECTINPUT8 lpDInput)
{
	if(FAILED(lpDInput->EnumDevices(DI8DEVCLASS_GAMECTRL,EnumJoyCallback,
			NULL,DIEDFL_ATTACHEDONLY))){
		return E_FAIL;
	}
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  DirectInputDeviceの列挙コールバック
//―――――――――――――――――――――――――――――――――――――――――//
BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* pdidInstance,VOID* pContext)
{
	DIDEVCAPS	diDevCaps;

	if(FAILED(lpDInput->CreateDevice(pdidInstance->guidInstance,
			&lpDIDev[cntIDev],NULL))){
		return DIENUM_CONTINUE;
	}
	diDevCaps.dwSize=sizeof(DIDEVCAPS);
	if(FAILED(lpDIDev[cntIDev]->GetCapabilities(&diDevCaps))){
		lpDIDev[cntIDev]->Release();
		lpDIDev[cntIDev]=NULL;
		return DIENUM_CONTINUE;
	}
	else{
		nBtn[cntIDev]=diDevCaps.dwButtons;
	}
	if(++cntIDev!=eIDEVNUM){
		return DIENUM_CONTINUE;
	}
	return DIENUM_STOP;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  DirectInput各種設定
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT SetupDI(HWND hWnd,int i)
{
	DIPROPDWORD		prop;

	if(lpDIDev[i]!=NULL){
		if(FAILED(lpDIDev[i]->SetDataFormat(&c_dfDIJoystick))){
			return E_FAIL;
		}
		if(FAILED(lpDIDev[i]->SetCooperativeLevel(hWnd,DISCL_NONEXCLUSIVE | DISCL_FOREGROUND))){
			return E_FAIL;
		}
		cntIDev=i;
		if(FAILED(lpDIDev[i]->EnumObjects(EnumAxisCallback,NULL,DIDFT_AXIS))){
			return E_FAIL;
		}
		prop.diph.dwSize		=sizeof(prop);
		prop.diph.dwHeaderSize	=sizeof(prop.diph);
		prop.diph.dwObj			=0;
		prop.diph.dwHow			=DIPH_DEVICE;
		prop.dwData				=DIPROPAXISMODE_ABS;
		if(FAILED(lpDIDev[i]->SetProperty(DIPROP_AXISMODE,&prop.diph))){
			return E_FAIL;
		}
		prop.dwData				=1000;
		if(FAILED(lpDIDev[i]->SetProperty(DIPROP_BUFFERSIZE,&prop.diph))){
			return E_FAIL;
		}
		lpDIDev[i]->Acquire();
	}
	else{
		return E_FAIL;
	}

	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  JoyStickの軸を列挙するコールバック
//―――――――――――――――――――――――――――――――――――――――――//
BOOL CALLBACK EnumAxisCallback(LPCDIDEVICEOBJECTINSTANCE lpInst,VOID *pRef)
{
	DIPROPRANGE		prg;

	ZeroMemory(&prg,sizeof(prg));
	prg.diph.dwSize=sizeof(prg);
	prg.diph.dwHeaderSize=sizeof(prg.diph);
	prg.diph.dwObj=lpInst->dwType;
	prg.diph.dwHow=DIPH_BYID;
	prg.lMin= -1000;
	prg.lMax= 1000;
	if(FAILED(lpDIDev[cntIDev]->SetProperty(DIPROP_RANGE,&prg.diph))){
		return DIENUM_STOP;
	}
	return DIENUM_CONTINUE;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  DirectInput関連のリリース
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseDI(LPDIRECTINPUT8 *pDInput,LPDIRECTINPUTDEVICE8 *pDIDev)
{
	for(int i=0;i<eIDEVNUM;i++){
		if(pDIDev[i]!=NULL){
			(pDIDev[i])->Unacquire();
			(pDIDev[i])->Release();
			pDIDev[i]=NULL;
		}
	}
	if(*pDInput!=NULL){
		(*pDInput)->Release();
		*pDInput=NULL;
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  描画開始
//―――――――――――――――――――――――――――――――――――――――――//
void BeginDraw(void)
{
	lpD3DDev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,BACK_COLOR,1.0,0);
	lpD3DDev->BeginScene();
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

void	Gs_DrawClear(void)	{	lpD3DDev->Clear(0,NULL,D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,BACK_COLOR,1.0,0);	}
void	Gs_DrawClear2(void)	{	lpD3DDev->Clear(0,NULL, D3DCLEAR_ZBUFFER,BACK_COLOR,1.0,0);	}
void	Gs_DrawScene(void)	{
	lpD3DDev->BeginScene();	
}

void	Gs_Begin2D(void)	{	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);		}

#ifdef DX3_PLUS
void	Gs_Begin3D(void)	{	lpSprite->End();Gs_3DSetUP( );	/*lpD3DDev->BeginScene();*/	}
#endif

void	Gs_Begin2DFront(void)
{	
	lpSprite->End();
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);	
}

void	Gs_Begin2DBack(void)
{
	lpSprite->End();
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	lpD3DDev->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
}

void	Gs_BeginAlPha(void)
{
	lpSprite->End();
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
	lpD3DDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	lpD3DDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	lpD3DDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  描画終了
//―――――――――――――――――――――――――――――――――――――――――//
void EndDraw(void)
{
	lpSprite->End();
	lpD3DDev->EndScene();
	lpD3DDev->Present(NULL,NULL,NULL,NULL);
}

void	Gs_DrawPresent(void)
{
	lpD3DDev->EndScene();
	lpD3DDev->Present(NULL,NULL,NULL,NULL);
}

void	Gs_End2D(void)			{	lpSprite->End();		}
void	Gs_End3D(void)			{	lpD3DDev->EndScene();	}
void	Gs_EndAlPha(void)
{
	lpSprite->End();
	lpSprite->Begin(D3DXSPRITE_ALPHABLEND);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Direct3Dの生成
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateD3D(LPDIRECT3D9 *pD3D)
{
	if(((*pD3D)=Direct3DCreate9(D3D_SDK_VERSION)) == NULL)	return E_FAIL;
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Direct3Dデバイスの生成(Windowモード)
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateD3DDevWin(HWND hWnd,LPDIRECT3D9 lpD3D,LPDIRECT3DDEVICE9 *pD3DDev)
{
	D3DDISPLAYMODE			dispMode;
	D3DPRESENT_PARAMETERS	presentParam;

	lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dispMode);
	ZeroMemory(&presentParam,sizeof(presentParam));
	presentParam.Windowed=TRUE;
	presentParam.BackBufferFormat	=dispMode.Format;
	presentParam.BackBufferCount=1;
	presentParam.SwapEffect=D3DSWAPEFFECT_DISCARD;
	presentParam.EnableAutoDepthStencil=TRUE;
	presentParam.AutoDepthStencilFormat=D3DFMT_D16;
	if(FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
			hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&presentParam,&(*pD3DDev))))
		return E_FAIL;
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Direct3Dデバイスの生成(FullScreenモード)
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateD3DDevFull(HWND hWnd,LPDIRECT3D9 lpD3D,LPDIRECT3DDEVICE9 *pD3DDev)
{
	D3DDISPLAYMODE			dispMode;
	D3DPRESENT_PARAMETERS	presentParam;

	lpD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&dispMode);
	ZeroMemory(&presentParam,sizeof(presentParam));
	presentParam.Windowed=FALSE;
	presentParam.BackBufferWidth=640;
	presentParam.BackBufferHeight=480;
	presentParam.BackBufferFormat=dispMode.Format;
	presentParam.BackBufferCount=1;
	presentParam.SwapEffect=D3DSWAPEFFECT_DISCARD;
	presentParam.FullScreen_RefreshRateInHz=D3DPRESENT_RATE_DEFAULT;
	presentParam.EnableAutoDepthStencil=TRUE;
	presentParam.AutoDepthStencilFormat=D3DFMT_D16;
	if(FAILED(lpD3D->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,
		hWnd,D3DCREATE_SOFTWARE_VERTEXPROCESSING,&presentParam,&(*pD3DDev))))
		return E_FAIL;
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Spriteの生成	
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateSprite(LPDIRECT3DDEVICE9 lpD3DDev,LPD3DXSPRITE *pSprite)
{
	if(FAILED(D3DXCreateSprite(lpD3DDev,pSprite)))
		return E_FAIL;
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Fontの生成
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateFont(LPDIRECT3DDEVICE9 lpD3DDev,LPD3DXFONT *pFont)
{

	if(FAILED(D3DXCreateFont(
		lpD3DDev,						//LPDIRECT3DDEVICE9	lpD3DDev,
		SYS_FONT_SIZE,						//UINT				Height,
		0,								//UINT				Width,
		FW_NORMAL,						//UINT				Weight,
		D3DX_DEFAULT,					//UINT				MipLevels,
		FALSE,							//BOOL				Italic,
		SHIFTJIS_CHARSET,				//DWORD				CharSet,
		OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
		PROOF_QUALITY,					//DWORD				Quality,
		DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
		&Font_Type[SYS_FONT_TYPE][0],	//LPCTSTR			pFacename,
		&lpFont[0]						//LPD3DXFONT		*ppFont
	))){
		return E_FAIL;
	}


	if (Gs_FontSet(FONT_TYPE,FONT_SIZE))	return S_OK;
	return E_FAIL;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  サーフェイスの生成
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateSF(LPDIRECTDRAWSURFACE *inSF1,LPDIRECTDRAWSURFACE *inSF2,int inW,int inH,HWND hWnd)
{
	//ダイレクトドローオブジェクトの生成
	if(FAILED(DirectDrawCreate(NULL,&(lpDDraw),NULL)))	return E_FAIL;

	//協調レベルの設定
	if(FAILED(lpDDraw->SetCooperativeLevel(hWnd,DDSCL_NORMAL))) return E_FAIL;
	
	DDSURFACEDESC       ddsd;

    ZeroMemory(&ddsd,sizeof(ddsd));
    ddsd.dwSize			=sizeof(ddsd);
	ddsd.dwFlags		=DDSD_CAPS|DDSD_HEIGHT|DDSD_WIDTH;
	ddsd.ddsCaps.dwCaps	=DDSCAPS_OFFSCREENPLAIN;
	ddsd.dwWidth		=inW;
	ddsd.dwHeight		=inH;
    if(lpDDraw->CreateSurface(&ddsd,inSF1,NULL) != DD_OK){
		return FALSE;
	}
    if(lpDDraw->CreateSurface(&ddsd,inSF2,NULL) != DD_OK){
		return FALSE;
	}

	return TRUE;
}


//―――――――――――――――――――――――――――――――――――――――――//
//  頂点バッファの生成
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT CreateVB(LPDIRECT3DDEVICE9 lpD3DDev, LPDIRECT3DVERTEXBUFFER9* pVertexBuffer)
{
	if(FAILED(lpD3DDev->CreateVertexBuffer(
		4*sizeof(MYVERTEX), 0, D3DFVF_VERTEX, 
		D3DPOOL_MANAGED, pVertexBuffer, NULL
	))){
		return E_FAIL;
	}
	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Direct3D関連のリリース
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseD3D(LPDIRECT3D9 *pD3D,LPDIRECT3DDEVICE9 *pD3DDev)
{
	if(*pD3DDev!=NULL){
		(*pD3DDev)->Release();
		(*pD3DDev)=NULL;
	}
	if(*pD3D!=NULL){
		(*pD3D)->Release();
		(*pD3D)=NULL;
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Spriteのリリース
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseSprite(LPD3DXSPRITE *pSprite)
{
	if((*pSprite)!=NULL){
		(*pSprite)->Release();
		(*pSprite)=NULL;
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Surfaceのリリース
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseSF(LPDIRECTDRAWSURFACE *pSF)
{
	if (lpDDraw!=NULL) {
		if((*pSF)!=NULL){
			(*pSF)->Release();
			(*pSF)=NULL;
		}
		if (lpDDSBack!=NULL) {
			lpDDSBack->Release();
			lpDDSBack = NULL;
		}

		lpDDraw->Release();
	}
}


//―――――――――――――――――――――――――――――――――――――――――//
//  頂点バッファーのリリース
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseVB(LPDIRECT3DVERTEXBUFFER9	*pVB)
{
	if(*pVB!=NULL){
		(*pVB)->Release();
		(*pVB)=NULL;
	}
}
//―――――――――――――――――――――――――――――――――――――――――//
//  Fontのリリース
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseFont(LPD3DXFONT *pFont)
{
	if((*pFont)!=NULL){
		(*pFont)->Release();
		(*pFont)=NULL;
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  環境設定
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT InitEnv(HWND hWnd)
{
	//環境の初期化
	lpD3D		= NULL;
	lpD3DDev	= NULL;
	lpSprite	= NULL;
	lpDInput	= NULL;
	lpDDraw		= NULL;


	lpVertexBuffer = NULL;


	for(int i=0;i<eIDEVNUM;i++)		lpDIDev[i]	=NULL;
	cntIDev		=0;

	//環境の生成
	//Graphics
	if(FAILED(CreateD3D(&lpD3D)))							return E_FAIL;

	if (FullWinSW == IDYES) {
		if(FAILED(CreateD3DDevFull(hWnd,lpD3D,&lpD3DDev)))	return E_FAIL;
	}
	else {
		if(FAILED(CreateD3DDevWin(hWnd,lpD3D,&lpD3DDev)))	return E_FAIL;
	}

	// Sprite
	if(FAILED(CreateSprite(lpD3DDev,&lpSprite)))
		return E_FAIL;

	// Surface
//	if(FAILED(CreateSF(&lpDDSPrim,&lpDDSBack,WINW,WINH,hWnd)))
//		return E_FAIL;

	//VertexBuffer
	if(FAILED(CreateVB(lpD3DDev, &lpVertexBuffer))) return E_FAIL;


	// Font
	if(FAILED(CreateFont(lpD3DDev,&lpFont[0])))	return E_FAIL;

	// DirectInput 
	if(FAILED(CreateDInput(hWnd,&lpDInput)))	return E_FAIL;

	// DirectInputDevice 
	if(FAILED(CreateDIDev(hWnd,lpDInput)))		return E_FAIL;

	// MUSIC
	if (FAILED(CreateDMusic(hWnd)))				return E_FAIL;


	//ゲームパッドの設定
	Pad[eIDEV0].Info = OK;
	if(FAILED(SetupDI(hWnd,eIDEV0))) {
		Pad[eIDEV0].Info = NG;
		return DX_ERR_SetupDI;
	}
#ifdef PAD4_USE
	Pad[eIDEV1].Info = OK;
	if(FAILED(SetupDI(hWnd,eIDEV1))) {
		Pad[eIDEV1].Info = NG;
		return DX_ERR_SetupDI;
	}

	Pad[eIDEV2].Info = OK;
	if(FAILED(SetupDI(hWnd,eIDEV2))) {
		Pad[eIDEV2].Info = NG;
		return DX_ERR_SetupDI;
	}

	Pad[eIDEV3].Info = OK;
	if(FAILED(SetupDI(hWnd,eIDEV3))) {
		Pad[eIDEV3].Info = NG;
		return DX_ERR_SetupDI;
	}
#endif

	return S_OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//  環境を閉じる																	//
//―――――――――――――――――――――――――――――――――――――――――//
void CloseEnv(void)
{
	//環境の破棄
	//DirectGraphics関連オブジェクトのリリース
	ReleaseSprite(&lpSprite);			// Sprite
//	ReleaseSF(&lpDDSPrim);				// Surface
	ReleaseVB(&lpVertexBuffer);
	ReleaseFont(&lpFont[0]);			// Font1
	ReleaseFont(&lpFont[1]);			// Font2
	ReleaseD3D(&lpD3D,&lpD3DDev);
	ReleaseDMusic( );					// Music

	//DirectInput関連オブジェクトのリリース
	ReleaseDI(&lpDInput,lpDIDev);
}

//―――――――――――――――――――――――――――――――――――――――――//
//  Textureの読み込み												 		  //
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT LoadTexture(LPCTSTR fName,TEX9 *pTex)
{

	if(FAILED(D3DXCreateTextureFromFileEx(
		lpD3DDev,				//LPDIRECT3DDEVICE9		pDevice,
		fName,					//LPCTSTR				pSrcFile,
		D3DX_DEFAULT,			//UINT					Width,
		D3DX_DEFAULT,			//UINT					Height,
		D3DX_DEFAULT,			//UINT					MipLevels,
		0,						//DWORD					Usage,
		D3DFMT_UNKNOWN,			//D3DFORMAT				Format,
		D3DPOOL_MANAGED,		//D3DPOOL				Pool,
		D3DX_DEFAULT,			//DWORD					Filter,
		D3DX_DEFAULT,			//DWORD					MipFilter,
		TRANS_COLOR,			//D3DCOLOR				ColorKey,	//抜け色を設定
		NULL,					//D3DXIMAGE_INFO		*pSrcInfo,
		NULL,					//PALETTEENTRY			*pPalette,
		pTex					//LPDIRECT3DTEXTURE9	*ppTexture
		))){
		return E_FAIL;
	}

	return S_OK;
}


HRESULT LoadResource(LPCTSTR fName,TEX9 *pTex)
{

	if(FAILED(D3DXCreateTextureFromResourceEx(
		lpD3DDev,				//LPDIRECT3DDEVICE9		pDevice,
		NULL,
		fName,					//LPCTSTR				pSrcFile,
		D3DX_DEFAULT,			//UINT					Width,
		D3DX_DEFAULT,			//UINT					Height,
		D3DX_DEFAULT,			//UINT					MipLevels,
		0,						//DWORD					Usage,
		D3DFMT_UNKNOWN,			//D3DFORMAT				Format,
		D3DPOOL_MANAGED,		//D3DPOOL				Pool,
		D3DX_DEFAULT,			//DWORD					Filter,
		D3DX_DEFAULT,			//DWORD					MipFilter,
		TRANS_COLOR,			//D3DCOLOR				ColorKey,	//抜け色を設定
		NULL,					//D3DXIMAGE_INFO		*pSrcInfo,
		NULL,					//PALETTEENTRY			*pPalette,
		pTex					//LPDIRECT3DTEXTURE9	*ppTexture
		))){
		return E_FAIL;
	}

	return S_OK;
}


//―――――――――――――――――――――――――――――――――――――――――//
//  Textureのリリース																//
//―――――――――――――――――――――――――――――――――――――――――//
void ReleaseTexture(TEX9 *pTex)
{
	if(*pTex!=NULL){
		(*pTex)->Release();
		*pTex=NULL;
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//  テクスチャーの描画																//
//―――――――――――――――――――――――――――――――――――――――――//
void DrawTexture(TEX9 pTex,POINT *pPos,RECT *pRect,float degree,POINT	*pCenter,float scaleX,float scaleY,GsCOLOR argb)
{
	D3DXMATRIX		mat,matID,matPos,matRot,matScale;
	D3DXVECTOR3		center;

	//単位行列の作成
	D3DXMatrixIdentity(&matID);

	//位置行列の作成
	D3DXMatrixTranslation(&matPos,(float)pPos->x,(float)pPos->y,0.0f);

	//回転行列の作成
	D3DXMatrixRotationZ(&matRot,D3DXToRadian(-degree));

	//拡大縮小行列の作成
	D3DXMatrixScaling(&matScale,scaleX,scaleY,1.0f);

	//行列の合成
	D3DXMatrixMultiply(&mat,&matID,&matScale);
	D3DXMatrixMultiply(&mat,&mat,&matRot);
	D3DXMatrixMultiply(&mat,&mat,&matPos);

	//スプライトエンジンに行列を登録
	lpSprite->SetTransform(&mat);

	//センターの作成
	center=D3DXVECTOR3((float)pCenter->x,(float)pCenter->y,0.0f);

	//描画
	lpSprite->Draw(pTex,pRect,&center,NULL,argb);

	//ワールドのリセット
	D3DXMatrixIdentity(&mat);
	lpSprite->SetTransform(&mat);
}



//―――――――――――――――――――――――――――――――――――――――――//
//	テクスチャー　反転描画													//
//―――――――――――――――――――――――――――――――――――――――――//
void DrawTextureR(TEX9 pTex,POINT *pPos,RECT *pRect,float degree,POINT *pCenter,float scaleX,float scaleY,GsCOLOR argb)
{
	D3DXMATRIX		mat,matPos,matRot,matScale;
	D3DXVECTOR3		center;

	//位置行列の作成
	D3DXMatrixTranslation(&matPos,(float)pPos->x,(float)pPos->y,0.0f);

	//回転行列の作成
	D3DXMatrixRotationYawPitchRoll(&matRot,(float)D3DXToRadian(180),(float)D3DXToRadian(0),(float)D3DXToRadian(-degree));

	//拡大縮小行列の作成
	D3DXMatrixScaling(&matScale,scaleX,scaleY,1.0f);

	//行列の合成
	D3DXMatrixMultiply(&mat,&matRot,&matScale);
	D3DXMatrixMultiply(&mat,&mat,&matPos);

	//スプライトエンジンに行列を登録
	lpSprite->SetTransform(&mat);

	//センターの作成
	center=D3DXVECTOR3((float)pCenter->x,(float)pCenter->y,0.0f);

	//描画
	lpSprite->Draw(pTex,pRect,&center,NULL,argb);

	// REVERSE
	//回転行列の作成
	D3DXMatrixRotationYawPitchRoll(&matRot,(float)D3DXToRadian(-180),(float)D3DXToRadian(0),(float)D3DXToRadian(-degree));

	//行列の合成
	D3DXMatrixMultiply(&mat,&matRot,&matScale);
	D3DXMatrixMultiply(&mat,&mat,&matPos);

	//スプライトエンジンに行列を登録
	lpSprite->SetTransform(&mat);

	//センターの作成
	center=D3DXVECTOR3((float)pCenter->x,(float)pCenter->y,0.0f);

	//描画
	lpSprite->Draw(pTex,pRect,&center,NULL,argb);

	//ワールドのリセット
	D3DXMatrixIdentity(&mat);
	lpSprite->SetTransform(&mat);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	テクスチャー　反転描画															//
//―――――――――――――――――――――――――――――――――――――――――//
void DrawDirectTextureR(TEX9 pTex,POINT	*Pos,RECT *Rect,GsCOLOR argb)
{
	D3DXMATRIX		mat,matPos,matRot,matScale;
	D3DXVECTOR3		center;

	//センターの作成
	D3DXVECTOR3	VGen = D3DXVECTOR3( F((Rect->right - Rect->left)/2),F(0),F(0));

	//位置行列の作成
	D3DXMatrixTranslation(&matPos,(float)Pos->x+((Rect->right - Rect->left)/2),(float)Pos->y,0.0f);

	//回転行列の作成
	D3DXMatrixRotationYawPitchRoll(&matRot,(float)D3DXToRadian(180),(float)D3DXToRadian(0),(float)D3DXToRadian(0));

	//行列の合成
	D3DXMatrixMultiply(&mat,&matRot,&matPos);

	//スプライトエンジンに行列を登録
	lpSprite->SetTransform(&mat);

	//描画
	lpSprite->Draw(pTex,Rect,&VGen,NULL,argb);

	// REVERSE
	//回転行列の作成
	D3DXMatrixRotationYawPitchRoll(&matRot,(float)D3DXToRadian(-180),(float)D3DXToRadian(0),(float)D3DXToRadian(0));

	//行列の合成
	D3DXMatrixMultiply(&mat,&matRot,&matPos);

	//スプライトエンジンに行列を登録
	lpSprite->SetTransform(&mat);

	//描画
	lpSprite->Draw(pTex,Rect,&VGen,NULL,argb);

	//ワールドのリセット	拡大縮小リセット	回転リセット
	D3DXMatrixIdentity(&mat);
	lpSprite->SetTransform(&mat);

}

//―――――――――――――――――――――――――――――――――――――――――//
//	テクスチャー　そのまま描画														//
//―――――――――――――――――――――――――――――――――――――――――//
void DrawDirectTexture(TEX9 Tex,POINT *Pos,RECT *Rect,GsCOLOR argb)
{
	D3DXVECTOR3	VPos = D3DXVECTOR3( F(Pos->x),F(Pos->y),F(0));
	D3DXVECTOR3	VGen = D3DXVECTOR3( F(0),F(0),F(0) );

	lpSprite->Draw(Tex,Rect,&VGen,&VPos,argb);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	文字列描画　テキスト表示														//
//―――――――――――――――――――――――――――――――――――――――――//
void DrawTextOut(int x,int y,char *str,GsCOLOR argb,int no,int size)
{
	RECT			rect;
	int				len = (int)strlen(str);
	
	if (no==1)	SetRect(&rect,x,y,x+(size*len),y+size);
	else		SetRect(&rect,x,y,x+(SYS_FONT_SIZE*len),y+SYS_FONT_SIZE);

	lpFont[no]->DrawText(
		lpSprite,			//LPD3DXSPRITE pSprite,
		str,				//LPCTSTR pString,
		-1,					//INT Count,
		&rect,				//LPRECT pRect,
		DT_LEFT,			//DWORD Format,
		argb				//D3DCOLOR Color
	);

}

//―――――――――――――――――――――――――――――――――――――――――//
//  パッド情報の取得                                                           //
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT	GetPadInfo(int id)
{

	HRESULT			hr;
	DIJOYSTATE		dijs;

	//キーフラグのクリア
	Pad[id].Old		= Pad[id].Now;
	Pad[id].Now		= 0;

	if (Pad[id].Info==NG)	return NG;

	//入力状態の取得準備
	if(FAILED(lpDIDev[id]->Poll())){
		hr=lpDIDev[id]->Acquire();
		while(hr==DIERR_INPUTLOST)
			hr=lpDIDev[id]->Acquire();
		return S_OK;
	}

	//入力状態の取得
	if(FAILED(lpDIDev[id]->GetDeviceState(sizeof(DIJOYSTATE),&dijs))) {
		Pad[id].Info = NG;
		return S_OK;
	}

	// Start Select
	if(dijs.rgbButtons[nBtn[id]-2] & 0x80)		Pad[id].Now	|= kPADSTART;
	if(dijs.rgbButtons[nBtn[id]-1] & 0x80)		Pad[id].Now	|= kPADSELECT;

	// ボタン
	if(dijs.rgbButtons[0] & 0x80)		Pad[id].Now	|= kPAD_A;
	if(dijs.rgbButtons[1] & 0x80)		Pad[id].Now	|= kPAD_B;
	if(dijs.rgbButtons[2] & 0x80)		Pad[id].Now	|= kPAD_C;
	if(dijs.rgbButtons[3] & 0x80)		Pad[id].Now	|= kPAD_X;
	if(dijs.rgbButtons[4] & 0x80)		Pad[id].Now	|= kPAD_Y;
	if(dijs.rgbButtons[5] & 0x80)		Pad[id].Now	|= kPAD_Z;
	if(dijs.rgbButtons[6] & 0x80)		Pad[id].Now	|= kPAD_L;
	if(dijs.rgbButtons[7] & 0x80)		Pad[id].Now	|= kPAD_R;

	// 十字キー（軸）
	if(dijs.lX >  900)					Pad[id].Now	|= kKEYRIGHT;
	if(dijs.lX < -900)					Pad[id].Now	|= kKEYLEFT;
	if(dijs.lY >  900)					Pad[id].Now	|= kKEYDOWN;
	if(dijs.lY < -900)					Pad[id].Now	|= kKEYUP;

	Pad[id].Trg		= (Pad[id].Now ^ Pad[id].Old) & (~Pad[id].Old);
	Pad[id].Pul		= (Pad[id].Now ^ Pad[id].Old) & (Pad[id].Old);
	
	return S_OK;
}

// パッド情報　取得　
short	GetPadNow(int id)	{	return(Pad[id].Now);	}
short	GetPadTrg(int id)	{	return(Pad[id].Trg);	}
short	GetPadPul(int id)	{	return(Pad[id].Pul);	}

//―――――――――――――――――――――――――――――――――――――――――//
//  キー情報の取得                                                                  //
//―――――――――――――――――――――――――――――――――――――――――//
HRESULT	GetKeyInfo(void)
{
	//キーフラグのクリア
	Key.Old		= Key.Now;
	Key.Now		= 0;

	//キー情報
	if (HIBYTE(GetAsyncKeyState(VK_UP    )) == 0x80)		Key.Now	|= kKEYUP;
	if (HIBYTE(GetAsyncKeyState(VK_DOWN  )) == 0x80)		Key.Now	|= kKEYDOWN;
	if (HIBYTE(GetAsyncKeyState(VK_LEFT  )) == 0x80)		Key.Now	|= kKEYLEFT;
	if (HIBYTE(GetAsyncKeyState(VK_RIGHT )) == 0x80)		Key.Now	|= kKEYRIGHT;
	if (HIBYTE(GetAsyncKeyState(VK_SPACE )) == 0x80)		Key.Now	|= kKEYSPACE;
	if (HIBYTE(GetAsyncKeyState(VK_RETURN)) == 0x80)		Key.Now	|= kKEYENTER;
	if (HIBYTE(GetAsyncKeyState('A'))		== 0x80)		Key.Now	|= kKEY_A;
	if (HIBYTE(GetAsyncKeyState('B'))		== 0x80)		Key.Now	|= kKEY_B;
	if (HIBYTE(GetAsyncKeyState('C'))		== 0x80)		Key.Now	|= kKEY_C;
	if (HIBYTE(GetAsyncKeyState('X'))		== 0x80)		Key.Now	|= kKEY_X;
	if (HIBYTE(GetAsyncKeyState('Y'))		== 0x80)		Key.Now	|= kKEY_Y;
	if (HIBYTE(GetAsyncKeyState('Z'))		== 0x80)		Key.Now	|= kKEY_Z;
	if (HIBYTE(GetAsyncKeyState(VK_F1    )) == 0x80)		Key.Now	|= kKEY_F1;
	if (HIBYTE(GetAsyncKeyState(VK_F2    )) == 0x80)		Key.Now	|= kKEY_F2;
	if (HIBYTE(GetAsyncKeyState(VK_F3    )) == 0x80)		Key.Now	|= kKEY_F3;
	if (HIBYTE(GetAsyncKeyState(ESC      )) == 0x80)		Key.Now	|= kKEYESC;

	//キー入力状態
	Key.Trg		= (Key.Now ^ Key.Old) & (~Key.Old);
	Key.Pul		= (Key.Now ^ Key.Old) & (Key.Old);
	
	return S_OK;
}

// キー情報　取得　
u_long		GetKeyNow(void)	{	return(Key.Now);	}
u_long		GetKeyTrg(void) {	return(Key.Trg);	}
u_long		GetKeyPul(void) {	return(Key.Pul);	}


// フルキー取得
void	FullKEYInfo(void)
{
	//キーフラグのクリア
	FullKEY.Old		= FullKEY.Now;
	FullKEY.Now		= 0;

	//キー情報
	if (HIBYTE(GetAsyncKeyState('A'))		== 0x80)		FullKEY.Now	|= tKEY_A;
	if (HIBYTE(GetAsyncKeyState('B'))		== 0x80)		FullKEY.Now	|= tKEY_B;
	if (HIBYTE(GetAsyncKeyState('C'))		== 0x80)		FullKEY.Now	|= tKEY_C;
	if (HIBYTE(GetAsyncKeyState('D'))		== 0x80)		FullKEY.Now	|= tKEY_D;
	if (HIBYTE(GetAsyncKeyState('E'))		== 0x80)		FullKEY.Now	|= tKEY_E;
	if (HIBYTE(GetAsyncKeyState('F'))		== 0x80)		FullKEY.Now	|= tKEY_F;
	if (HIBYTE(GetAsyncKeyState('G'))		== 0x80)		FullKEY.Now	|= tKEY_G;
	if (HIBYTE(GetAsyncKeyState('H'))		== 0x80)		FullKEY.Now	|= tKEY_H;
	if (HIBYTE(GetAsyncKeyState('I'))		== 0x80)		FullKEY.Now	|= tKEY_I;
	if (HIBYTE(GetAsyncKeyState('J'))		== 0x80)		FullKEY.Now	|= tKEY_J;
	if (HIBYTE(GetAsyncKeyState('K'))		== 0x80)		FullKEY.Now	|= tKEY_K;
	if (HIBYTE(GetAsyncKeyState('L'))		== 0x80)		FullKEY.Now	|= tKEY_L;
	if (HIBYTE(GetAsyncKeyState('M'))		== 0x80)		FullKEY.Now	|= tKEY_M;
	if (HIBYTE(GetAsyncKeyState('N'))		== 0x80)		FullKEY.Now	|= tKEY_N;
	if (HIBYTE(GetAsyncKeyState('O'))		== 0x80)		FullKEY.Now	|= tKEY_O;
	if (HIBYTE(GetAsyncKeyState('P'))		== 0x80)		FullKEY.Now	|= tKEY_P;
	if (HIBYTE(GetAsyncKeyState('Q'))		== 0x80)		FullKEY.Now	|= tKEY_Q;
	if (HIBYTE(GetAsyncKeyState('R'))		== 0x80)		FullKEY.Now	|= tKEY_R;
	if (HIBYTE(GetAsyncKeyState('S'))		== 0x80)		FullKEY.Now	|= tKEY_S;
	if (HIBYTE(GetAsyncKeyState('T'))		== 0x80)		FullKEY.Now	|= tKEY_T;
	if (HIBYTE(GetAsyncKeyState('U'))		== 0x80)		FullKEY.Now	|= tKEY_U;
	if (HIBYTE(GetAsyncKeyState('V'))		== 0x80)		FullKEY.Now	|= tKEY_V;
	if (HIBYTE(GetAsyncKeyState('W'))		== 0x80)		FullKEY.Now	|= tKEY_W;
	if (HIBYTE(GetAsyncKeyState('X'))		== 0x80)		FullKEY.Now	|= tKEY_X;
	if (HIBYTE(GetAsyncKeyState('Y'))		== 0x80)		FullKEY.Now	|= tKEY_Y;
	if (HIBYTE(GetAsyncKeyState('Z'))		== 0x80)		FullKEY.Now	|= tKEY_Z;

	//キー入力状態
	FullKEY.Trg		= (FullKEY.Now ^ FullKEY.Old) & (~FullKEY.Old);
	FullKEY.Pul		= (FullKEY.Now ^ FullKEY.Old) & (FullKEY.Old);
}


//======================================================================================//
//																						//
//									GsLIB												//
//																						//
//					  Game Simplicity Library　For Windows								//
//						   ( DxLIB  Rapper Function )									//
//																						//
//======================================================================================//

//----------------------------------------------------------------------------------//
//	Definition																		//
//----------------------------------------------------------------------------------//
#define		SYS_1		1
#define		SYS_2		2
#define		SYS_3		3

//#define		GS_SYSBMP		"DATA/SYSTEM/SYSTEM.GSB"
#define		GS_SYSBMP		"GC_SYSTEM"
//----------------------------------------------------------------------------------//
//	Gs Global Variable																//
//----------------------------------------------------------------------------------//

BMPHANG					GsBmpHang[GsBMPMAX];
CHRHANG					GsChrHang[GsCHRMAX];

static	short			GsFontNo;
static	short			GsFontSize;

HWND					GsHwnd;

//―――――――――――――――――――――――――――――――――――――――――//
//	Gs　初期化																		//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_LoadSYSTEM(void)
{
	GsBmpHang[SYS_1].Flg = ON;
//	if (FAILED(LoadTexture(GS_SYSBMP,&GsBmpHang[SYS_1].Bmp ))) {
	if (FAILED(LoadResource(GS_SYSBMP,&GsBmpHang[SYS_1].Bmp ))) {
		System_Message(GsHwnd,"システムＢＭＰのロードに失敗しました");
		GsBmpHang[SYS_1].Flg = OFF;
	}
}

BOOL	Gs_Init(HWND hWnd)
{
	int		i,err;


	// 環境設定
	FullWinSW = IDNO;
#ifdef	FULLSCREEN
	FullWinSW = System_MessageYN(hWnd,"フルスクリーンモードで起動しますか？");
#endif

	GsHwnd		= hWnd;
	err			= InitEnv(hWnd);

	switch(err)
	{
		case	S_OK:
			err	= OK;
			break;
		case	DX_ERR_SetupDI:
			err	= OK;				// NG;System_Message(hWnd,"パッドの認識できません");
			break;
		default:
			System_Message(hWnd,"DXの初期化ができません");
			err	= NG;
			break;
	}

	// Gs 初期化
	for (i=0;i<GsBMPMAX;i++){
		GsBmpHang[i].Flg 	= OFF;
	}

	for (i=0;i<GsCHRMAX;i++){
		GsChrHang[i].No			= 0;
	}

#ifdef DX3_PLUS
	// ３Ｄ初期化
	Gs_Init3D	( );
#endif
	// System BMP Load
	Gs_LoadSYSTEM( );

	return	(err);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	Gs　終了処理
//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_Exit(void)
{
	int		i;

	for (i=0;i<GsBMPMAX;i++){
		if (GsBmpHang[i].Flg) {
			ReleaseTexture(&GsBmpHang[i].Bmp);
		}
	}

#ifdef DX3_PLUS
	// ３Ｄ終了処理
	Gs_Exit3D	( );
#endif

	CloseEnv( );		//環境Reset
	return	(OK);
}


//―――――――――――――――――――――――――――――――――――――――――//
//	Gs Check PAD
//
//―――――――――――――――――――――――――――――――――――――――――//

// PADの入力データをセット　＆　PADの状態
BOOL	Gs_PADInfo(int id)	{	return(Pad[id].Info);	}

BOOL	Gs_CheckPAD(void)
{	
	GetPadInfo(eIDEV0);
#ifdef PAD4_USE
	GetPadInfo(eIDEV1);
	GetPadInfo(eIDEV2);
	GetPadInfo(eIDEV3);
#endif

	return(OK);				//	return(GetPadInfo(id));
}

// 指定したボタンが押されているか？
bool	Gs_GetPAD(int id,int x)		{	return(IB(GetPadNow(id)&x));	}

// 指定したボタンのトリガを取得する
bool	Gs_GetPADtrg(int id,int x)	{	return(IB(GetPadTrg(id)&x));	}

// 指定したボタンのリリースを取得する
bool	Gs_GetPADpul(int id,int x)	{	return(IB(GetPadPul(id)&x));	}

//―――――――――――――――――――――――――――――――――――――――――//
//	Gs Check KEY
//
//―――――――――――――――――――――――――――――――――――――――――//

// キーボードの入力データをセット　＆　キーボードの状態
BOOL	Gs_CheckKEY(void)	{	return(IB(GetKeyInfo( )));	}

// 指定したKEYが押されているか？
bool	Gs_GetVKEY(int x)
{
	return(IB(HIBYTE(GetAsyncKeyState(x))==0x80));
}

// 指定したボタンが押されているか？
bool	Gs_GetKEY(int x)	{	return(IB(GetKeyNow( )&x));	}

// 指定したボタンのトリガを取得する
bool	Gs_GetKEYtrg(int x)	{	return(IB(GetKeyTrg( )&x));	}

// 指定したボタンのリリース（離した状態）を取得する
bool	Gs_GetKEYpul(int x) {	return(IB(GetKeyPul( )&x));	}


//―――――――――――――――――――――――――――――――――――――――――//
//	Gs MOUSE
//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_MousePosSet(int x,int y)
{	
	Mouse.Px	= x;
	Mouse.Py	= y;
	Mouse.Ox	= x;
	Mouse.Oy	= y;
	Mouse.Dx	= 0;
	Mouse.Dy	= 0;
	x	= x + mWin.DispX;
	y	= y + mWin.DispY;
	SetCursorPos(x,y);
}

void	Gs_MouseArea(int sw)
{
	RECT	rect;

	switch(sw)
	{
		case	AREA_WIN:
			SetRect(&rect,mWin.DispX,mWin.DispY,mWin.DispX+WINW,mWin.DispY+WINH);
			ClipCursor(&rect);
			break;

		case	AREA_FULL:
			SetRect(&rect,0,0,mWin.Ex,mWin.Ey);
			break;
		
		case	AREA_FREE:
			break;

		default:
			break;
	}

}


void	Gs_MouseMoveArea(int sx,int sy,int ex,int ey)
{
	RECT	rect;

	SetRect(&rect,mWin.DispX+sx,mWin.DispY+sy,mWin.DispX+ex,mWin.DispY+ey);
	ClipCursor(&rect);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	Gs Draw (Bigin & End)
//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_BeginDraw(void)	{	BeginDraw	( );	}

void	Gs_EndDraw(void)
{
	Gs_FadeDraw	( );
	EndDraw		( );
}

//―――――――――――――――――――――――――――――――――――――――――//
//	BITMAPの読み込み
//
//―――――――――――――――――――――――――――――――――――――――――//
GsTEXTURE	Gs_LoadBMP(LPCTSTR fName)
{
	int		i,ret;

	for (i=GsBMPSTART; i<GsBMPMAX; i++){
		if (!GsBmpHang[i].Flg) {
			ret	= i;
			GsBmpHang[i].Flg = ON;
			if (FAILED(LoadTexture(fName,&GsBmpHang[i].Bmp ))) {
				System_Message(GsHwnd,"ロードに失敗しました : %s",fName);
				GsBmpHang[i].Flg = OFF;
				ret = NG;
			}
			break;
		}
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	BITMAPの破棄	
//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ReleaseBMP(int i)
{
	if (GsBmpHang[i].Flg) {
		GsBmpHang[i].Flg = OFF;
		ReleaseTexture(&GsBmpHang[i].Bmp);
	}
	return	(OK);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	テキスト文字列の描画
//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_DrawText(int x,int y,char *str,GsCOLOR argb)
{

	DrawTextOut(x,y,str,argb,1,GsFontSize);
	return	(OK);
}


//―――――――――――――――――――――――――――――――――――――――――//
//	FONT TYPE SELECT                                                          //
//                                                                            //
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_FontSet(int no,int size)
{

	if ( GsFontNo != no || GsFontSize != size ) {

		ReleaseFont(&lpFont[1]);

		if(FAILED(D3DXCreateFont(
			lpD3DDev,						//LPDIRECT3DDEVICE9	lpD3DDev,
			size,							//UINT				Height,
			0,								//UINT				Width,
			FW_NORMAL,						//UINT				Weight,
			D3DX_DEFAULT,					//UINT				MipLevels,
			FALSE,							//BOOL				Italic,
			SHIFTJIS_CHARSET,				//DWORD				CharSet,
			OUT_DEFAULT_PRECIS,				//DWORD				OutputPrecision,
			PROOF_QUALITY,					//DWORD				Quality,
			DEFAULT_PITCH | FF_DONTCARE,	//DWORD				PitchAndFamily,
			&Font_Type[no][0],				//LPCTSTR			pFacename,
			&lpFont[1]						//LPD3DXFONT		*ppFont
		))){
			return NG;
		}
		GsFontSize	= size;
		GsFontNo	= no;
	}
	return OK;
}

//―――――――――――――――――――――――――――――――――――――――――//
//	BITMAPをキャラ分割して登録														//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ChrSet(int no,GsTEXTURE BmpNo,int sx,int sy,int w,int h)
{
	BOOL	ret = NG;
	if (no < GsCHRMAX) {
		GsChrHang[no].No			= BmpNo;
		GsChrHang[no].Rect.left		= sx;
		GsChrHang[no].Rect.top		= sy;
		GsChrHang[no].Rect.right	= sx+w;
		GsChrHang[no].Rect.bottom 	= sy+h;

		GsChrHang[no].ScaleX		= 1.0f;
		GsChrHang[no].ScaleY		= 1.0f;
		GsChrHang[no].Rot			= 0;
		GsChrHang[no].GsRGB			= RGB(0xff,0xff,0xff);
		GsChrHang[no].Alpha			= 0xff;	
		GsChrHang[no].Stt			= ALIVE;

		ret		= OK;
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	登録キャラの描画	（原点）													//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ChrDraw(int no,int px,int py)
{
	BOOL	ret = NG;

	if (GsChrHang[no].Stt == ALIVE)	{

		POINT		pos		= {px,py};
		RECT		rect	= GsChrHang[no].Rect;
		float		scX		= GsChrHang[no].ScaleX;
		float		scY		= GsChrHang[no].ScaleY;
		int			rot		= GsChrHang[no].Rot;
		u_char		Alpha	= GsChrHang[no].Alpha;
		u_char		Bc		= (u_char)((GsChrHang[no].GsRGB & 0x00ff0000) >> 16);
		u_char		Gc		= (u_char)((GsChrHang[no].GsRGB & 0x0000ff00) >>  8);
		u_char		Rc		= (u_char)(GsChrHang[no].GsRGB & 0x000000ff);
		int			w		= rect.right - rect.left;
		int			h		= rect.bottom - rect.top;
		POINT		rotp	= {(int)(w/2),(int)(h/2)};
//		pos.x				= (int)(pos.x - ((w*scX)/2));
//		pos.y				= (int)(pos.y - ((h*scY)/2));

		no		= GsChrHang[no].No;

		if (GsBmpHang[no].Flg) {
			DrawTexture(GsBmpHang[no].Bmp ,&pos,&rect,F(rot),&rotp,scX,scY,ARGB(Alpha,Rc,Gc,Bc));
			ret	= OK;
		}
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	登録キャラの拡大縮小	（原点）												//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ChrEX(float sx,float sy,int no)
{
	BOOL	ret = NG;

	if (GsChrHang[no].Stt == ALIVE)	{
		GsChrHang[no].ScaleX	= sx;
		GsChrHang[no].ScaleY	= sy;
		ret		= OK;
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	登録キャラの回転	（原点）													//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ChrROT(int rot,int no)
{
	BOOL	ret = NG;

	if (GsChrHang[no].Stt == ALIVE)	{
		GsChrHang[no].Rot 	= rot;
		ret		= OK;
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	登録キャラの拡大縮小	（原点）												//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
BOOL	Gs_ChrAlpha(u_char alpha,int no)
{
	BOOL	ret = NG;

	if (GsChrHang[no].Stt == ALIVE)	{
		GsChrHang[no].Alpha 	= alpha;
		ret		= OK;
	}
	return	(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	BITMAPの描画	基本															//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_DrawDirectLayer(int px,int py,GsTEXTURE no,int x1,int y1,int w,int h,bool rev,GsCOLOR argb)
{
	POINT	pos		= {px,py};
	RECT	rect	= {x1,y1,x1+w,y1+h};

	if (rev)	DrawDirectTextureR(GsBmpHang[no].Bmp,&pos,&rect,argb);
	else		DrawDirectTexture(GsBmpHang[no].Bmp,&pos,&rect,argb);
}

void	Gs_DrawLayer(int px,int py,GsTEXTURE no,int x1,int y1,int w,int h,bool rev,GsCOLOR argb,short gen,short rot,float scx,float scy)
{
	POINT		pos		= {px,py};
	RECT		rect	= {x1,y1,x1+w,y1+h};
	POINT		rotp	= {0,0};

	if (gen==1)	{	rotp.x = int(w/2);	rotp.y = int(h/2);	}	// center
	if (gen==2)	{	rotp.x = int(w/2);	rotp.y = int(0);	}	// Up center
	if (gen==3)	{	rotp.x = int(w/2);	rotp.y = int(h);	}	// Down center
	if (gen==4)	{	rotp.x = int(0);	rotp.y = int(h/2);	}	// Left center
	if (gen==5)	{	rotp.x = int(w);	rotp.y = int(h/2);	}	// Right center

	if (rev)	DrawTextureR(GsBmpHang[no].Bmp ,&pos,&rect,F(-rot),&rotp,scx,scy,argb);
	else		DrawTexture (GsBmpHang[no].Bmp ,&pos,&rect,F(-rot),&rotp,scx,scy,argb);
}



//―――――――――――――――――――――――――――――――――――――――――//
//	ライン描画																		//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_DrawLine(int sx,int sy,int len,float ang,GsCOLOR argb)
{
	POINT	pos		= {sx,sy};
	POINT	basep	= {0,0};
	RECT	rect	= {0,0,1,1};

	DrawTexture(GsBmpHang[SYS_1].Bmp,&pos,&rect,-ang,&basep,F(len),1.0f,argb);
}

void	Gs_DrawLineTo(int sx,int sy,int ex,int ey,GsCOLOR argb,int size)
{
	float		len = OBLIQ((ex-sx),(ey-sy));
	float		ang = ATANG((ex-sx),(ey-sy));
	LIMIT(size,0,60);

	POINT	pos		= {sx,sy};
	POINT	basep	= {0,size};
	RECT	rect	= {0,0,1,size*2 + 1};
	DrawTexture(GsBmpHang[SYS_1].Bmp,&pos,&rect,-ang,&basep,F(len),1.0f,argb);
}


//―――――――――――――――――――――――――――――――――――――――――//
//	ＢＯＸ描画																		//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_DrawBox(int sx,int sy,int ex,int ey,GsCOLOR argb,bool nuri)
{
	POINT	pos		= {sx,sy};
	POINT	basep	= {0,0};
	RECT	rect	= {0,0,31,31};
	int		w		= abs(ex-sx);
	int		h		= abs(ey-sy);

	if (nuri) {
		DrawTexture(GsBmpHang[SYS_1].Bmp,&pos,&rect,0,&basep,F(w)/31,F(h)/31,argb);
	}
	else {
		Gs_DrawLine(sx,sy,  w,  0,argb);
		Gs_DrawLine(sx,sy+h,w,  0,argb);
		Gs_DrawLine(sx,sy  ,h+1,90,argb);
		Gs_DrawLine(sx+w,sy,h+1,90,argb);
	}
}

//―――――――――――――――――――――――――――――――――――――――――//
//	点　描画																		//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_DrawPixel(int x,int y,GsCOLOR argb,int sc)
{
	POINT	pos;
	RECT	rect;
	int		sx,w;

	LIMIT(sc,0,8);
	pos.x	= x-sc;
	pos.y	= y-sc;
	sx		= 32 + sc*16;
	w		= sc*2 + 1;
	SetRect(&rect,sx,0,sx+w,w);
	DrawDirectTexture(GsBmpHang[SYS_1].Bmp,&pos,&rect,argb);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	円　描画																		//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_DrawCircle(int x,int y,float r,int line,GsCOLOR argb,bool nuri)
{
	POINT	pos		= {x,y};
	POINT	basep	= {128,128};
	RECT	rect	= {256,0,256+255,255};

	if (nuri) {
		DrawTexture(GsBmpHang[SYS_1].Bmp,&pos,&rect,0,&basep,F(r)/127.0f,F(r)/127.0f,argb);
		r		= r-3;
		line	= 2;
	}

	// 円周
	for (float i=0;i<360;i=i+0.5f) {
		pos.x	=	I(ROUND_X(i,r,x));
		pos.y	=	I(ROUND_Y(i,r,y));
		Gs_DrawPixel(pos.x,pos.y,argb,line);
	}
}

void	Gs_DrawArc(int x,int y,float r,int line,GsCOLOR argb,float s_ang,float e_ang)
{
	POINT	pos		= {x,y};
	POINT	basep	= {128,128};
	RECT	rect	= {256,0,256+255,255};

	// -359～360
	s_ang	= REMF(s_ang,360);
	e_ang	= REMF(e_ang,360);

	if (e_ang < 0)			e_ang = 360.0000f+e_ang;
	if (s_ang > e_ang)		s_ang = s_ang-360.0000f;		// 逆転補正
	if (e_ang-s_ang >= 360)	s_ang = s_ang+360.0000f;		// 逆転補正

	// 円周 s_ang（スタート角）から e_ang（エンド角）
	for (float i=s_ang;i<e_ang;i=i+0.5f) {
		pos.x	=	I(ROUND_X(i,r,x));
		pos.y	=	I(ROUND_Y(i,r,y));
		Gs_DrawPixel(pos.x,pos.y,argb,line);
	}
}


//―――――――――――――――――――――――――――――――――――――――――//
//	画面効果																		//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
enum	{	F_NONE,F_OUT,F_IN	};

struct {
	bool		Sw;
	short		Stt;
	COLORREF	Col;
	float		Cnt;
	float		Time;
} GsFadeHang;

void	Gs_FadeInit(void)
{
	GsFadeHang.Stt		= F_NONE;
	GsFadeHang.Sw		= OFF;
}

BOOL	Gs_FadeCheck(void)
{
	return(GsFadeHang.Sw);
}

void	Gs_FadeOut(COLORREF col,float time)
{
	if (GsFadeHang.Sw == OFF && GsFadeHang.Stt != F_OUT) {

		GsFadeHang.Cnt		= 0x00;
		GsFadeHang.Col		= col;
		GsFadeHang.Time		= time;
		GsFadeHang.Stt		= F_OUT;
		GsFadeHang.Sw		= ON;
	}
}

void	Gs_FadeIn(COLORREF col,float time)
{
	if (GsFadeHang.Sw == OFF && GsFadeHang.Stt != F_IN) {

		GsFadeHang.Cnt		= 0x00;
		GsFadeHang.Col		= col;
		GsFadeHang.Time		= time;
		GsFadeHang.Stt		= F_IN;
		GsFadeHang.Sw		= ON;
	}
}

void	Gs_FadeDraw(void)
{
	POINT	pos		= {0,0};
	POINT	basep	= {0,0};
	RECT	rect	= {0,0,32,32};
	u_char	Bc		= (u_char)((GsFadeHang.Col & 0x00ff0000) >> 16);
	u_char	Gc		= (u_char)((GsFadeHang.Col & 0x0000ff00) >>  8);
	u_char	Rc		= (u_char)( GsFadeHang.Col & 0x000000ff);

	if (GsFadeHang.Sw) {

		GsFadeHang.Cnt = GsFadeHang.Cnt + GsFadeHang.Time;
		if (GsFadeHang.Cnt >= 0xff)	{
			GsFadeHang.Cnt = 0xff;
			GsFadeHang.Sw  = OFF;
		}

		int		trans;
		if (GsFadeHang.Stt == F_IN)	trans	= 0xff - I(GsFadeHang.Cnt);		// IN
		else						trans	= I(GsFadeHang.Cnt);			// OUT

		DrawTexture(GsBmpHang[SYS_1].Bmp,&pos,&rect,0,&basep,F(WINW/32),F(WINH/32),ARGB(trans,Rc,Gc,Bc));
	}
}


//==========================================================================================//
//																							//
//										WinLIB												//
//																							//
//						  Game Standard Library　For Windows								//
//								 (Appropriationed Ewin)										//
//																							//
//==========================================================================================//

//―――――――――――――――――――――――――――――――――――――――――//
//																					//
//				固定フレームで同期させる											//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
static	DWORD	loopTime	= timeGetTime();
static	DWORD	waitTime	= 0;
static	DWORD	sFPS		= 0;
static	DWORD	fpsTime		= timeGetTime();
static	DWORD	syncCnt		= 0;

//--------------------------------------------------//
//	指定した間隔でＯＫを返す							//
//--------------------------------------------------//
BOOL	SyncWait(DWORD t)
{
	bool		Ret		= NG;
	DWORD		swait	= timeGetTime() - loopTime;

	// Syncタイム　計測
	if(swait >= (1000/t)){				// F(1000.0f/t)
		loopTime	= timeGetTime();
		waitTime	= swait;
		syncCnt		= syncCnt + 1;
		Ret			= OK;
	}

	// FPS 計測
	if (timeGetTime() - fpsTime > (1000-F(1000.0f/t))) {
		sFPS		= syncCnt;
		syncCnt		= 0;
		fpsTime		= timeGetTime();
	}
	return(Ret);
}

//	１フレームのタイム	----------------------------//
DWORD	SyncTime(void)	{	return	(waitTime);	}

//	1秒間の処理回数	--------------------------------//
DWORD	SyncFPS(void)	{	return	(sFPS);		}

//	FPS DISP		--------------------------------//
void	System_FPS(int x,int y)
{
	char	buf[16];

	wsprintf(buf," FPS = %d",SyncFPS( ));
	DrawTextOut(x,y,buf,ARGB(0xff,0xff,0x80,0x80),0,SYS_FONT_SIZE);
}


void	System_MakeWindow(char *cLass,char *tItle,int sTyle,int SizeX,int SizeY)
{

	hWnd=CreateWindowEx(WS_EX_APPWINDOW,cLass,tItle,sTyle,
		(GetSystemMetrics(SM_CXSCREEN)-
		(SizeX+GetSystemMetrics(SM_CXBORDER)+
		GetSystemMetrics(SM_CXEDGE)+
		GetSystemMetrics(SM_CXDLGFRAME)))/2,
		(GetSystemMetrics(SM_CYSCREEN)-
		(SizeY+GetSystemMetrics(SM_CYCAPTION)+
		GetSystemMetrics(SM_CYEDGE)+
		GetSystemMetrics(SM_CYBORDER)+
		GetSystemMetrics(SM_CYDLGFRAME)))/2,
		SizeX+GetSystemMetrics(SM_CXBORDER)+
		GetSystemMetrics(SM_CXEDGE)+
		GetSystemMetrics(SM_CXDLGFRAME),
		SizeY+GetSystemMetrics(SM_CYCAPTION)+
		GetSystemMetrics(SM_CYEDGE)+
		GetSystemMetrics(SM_CYBORDER)+
		GetSystemMetrics(SM_CYDLGFRAME),
		NULL,NULL,hInst,NULL);
}

//----------------------------------------------------------------------------------//
//		文字列操作																	//
//----------------------------------------------------------------------------------//

//―――――――――――――――――――――――――――――――――――――――――//
//				文字列の中に指定の文字があるか調べる								//
//																					//
//						char		*str : 調べられる文字列							//
//						char		code : 検索する文字								//
//						return		pointer of str :あり							//
//									NULL           :なし							//
//―――――――――――――――――――――――――――――――――――――――――//
char	*FindCHR(char *str,char code)
{
	for( ; *str != NULL ;str++){
		if (*str == code)	return(str);
	}
	return(NULL);
}

//―――――――――――――――――――――――――――――――――――――――――//
//				文字列の中に指定の文字列があるか調べる								//
//																					//
//						char		*str : 調べられる文字列							//
//						char		*key : 検索する文字列							//
//						return		pointer of str :あり							//
//									NULL           :なし							//
//―――――――――――――――――――――――――――――――――――――――――//
char	*FindSTR(char *str,char *key)
{
	char	*s1,*k1;

	for(;*str;str++){
		if(*key == *str){
			s1	=	str+1;
			k1	=	key+1;
			for(;*s1 !=NULL;s1++,k1++){
				if(*k1 == NULL)		return(str);
				if(*k1 != *s1)		break;
			}
			if(*k1 == NULL)		return(str);
        }
    }
	return(NULL);
}

//----------------------------------------------------------------------------------//
//	メッセージ 関連																	//
//----------------------------------------------------------------------------------//
//―――――――――――――――――――――――――――――――――――――――――//
//				デバッグ用メッセージの表示                                          //
//						char* Format ... : printf関数の書式と同様					//
//―――――――――――――――――――――――――――――――――――――――――//

#define		DEBMES_MAX			64
#define		DEBMESXY_MAX		256

struct {
	bool	Flg;
	short	Cnt;
	char	Mes[DEBMES_MAX][32];

	bool	FlgXY;
	short	CntXY;
	short	Px[DEBMES_MAX];
	short	Py[DEBMES_MAX];
	char	MesXY[DEBMES_MAX][16];
} SysDeb;


//	DEB Loop		----------------------------------------//
//
// DEBUG_DISPが定義されている状態　→　MasterがCall
//
//----------------------------------------------------------//
void	Gs_DebugInit(void)
{
	if (Gs_GetKEYtrg(kKEY_F1)) {
		SysDeb.Flg		= !SysDeb.Flg;
		SysDeb.FlgXY	= !SysDeb.FlgXY;
	}

	SysDeb.Cnt		= 0;
	SysDeb.CntXY	= 0;
}

//	DEB Disp		----------------------------------------//
//
//	EX: Gs_DebugOut("Move Flag = %d",Flg);
//
//----------------------------------------------------------//
void	Gs_DebugOut(char* Format,...)
{
	if (SysDeb.Cnt < DEBMES_MAX) {
		vsprintf_s(SysDeb.Mes[SysDeb.Cnt],Format,(char*)(&Format+1));
		SysDeb.Cnt++;
	}
}

//	DEB (x,y) Disp	----------------------------------------//
//
//	EX: Gs_DebugOutXY(100,100,"(%d,%d)",(int)Px,(int)Py);
//
//----------------------------------------------------------//
void	Gs_DebugOutXY(int x,int y,char* Format,...)
{
	if (SysDeb.CntXY < DEBMESXY_MAX) {
		vsprintf_s(SysDeb.MesXY[SysDeb.CntXY],Format,(char*)(&Format+1));
		SysDeb.Px[SysDeb.CntXY] = x;
		SysDeb.Py[SysDeb.CntXY] = y;
		SysDeb.CntXY++;
	}
}

//	DEB ALL Draw	----------------------------------------//
//
//	DEBUG_DISP	→　MasterがCall：デバッグ関連の一括描画
//
//----------------------------------------------------------//
void	Gs_DebugDraw(void)
{
	int		i,len;

	// Value Disp
	if (SysDeb.Flg) {

		System_FPS(16,0);

		for (i=0;i<SysDeb.Cnt;i++) {
			DrawTextOut(16+(i/16)*128,16+(i%16)*16,SysDeb.Mes[i],BETA_COLOR,0,SYS_FONT_SIZE);
		}
	}

	// X,Y Disp
	if (SysDeb.FlgXY) {
		for (i=0;i<SysDeb.CntXY;i++) {
			len = (int)strlen(SysDeb.MesXY[i])-1;
			Gs_DrawBox(SysDeb.Px[i],SysDeb.Py[i],SysDeb.Px[i]+(SYS_FONT_SIZE/2*len),SysDeb.Py[i]+SYS_FONT_SIZE,ARGB(0x80,0x10,0x10,0xe0),ON);
			DrawTextOut(SysDeb.Px[i],SysDeb.Py[i],SysDeb.MesXY[i],BETA_COLOR,0,SYS_FONT_SIZE);
		}
	}
}

void	Gs_SystemText(int x,int y,GsCOLOR argb,char* Format,...)
{
	char	Buf[32];
	wvsprintf(Buf,Format,(char*)(&Format+1));	
	DrawTextOut(x,y,Buf,argb,0,SYS_FONT_SIZE);
}

//―――――――――――――――――――――――――――――――――――――――――//
//				メッセージボックスの表示											//
//						char* Format ... : printf関数の書式と同様					//
//―――――――――――――――――――――――――――――――――――――――――//
void	System_Message(HWND hwnd, char* Format,...)
{
    char Buffer[64];

    vsprintf_s(Buffer,Format,(char*)(&Format+1));
    strcat_s(Buffer,"\r\n");
    MessageBox(hwnd,Buffer,"メッセージ",MB_OK|MB_SYSTEMMODAL|MB_ICONEXCLAMATION);
}

int		System_MessageYN(HWND hwnd, char* Format,...)
{
    char	Buffer[64];
	int		ret;

    vsprintf_s(Buffer,Format,(char*)(&Format+1));
    strcat_s(Buffer,"\r\n");
    ret = MessageBox(hwnd,Buffer,"メッセージ",MB_YESNO);
	return(ret);
}

//―――――――――――――――――――――――――――――――――――――――――//
//				ファイル書き込み　＆　読み込み										//
//																					//
//―――――――――――――――――――――――――――――――――――――――――//
static		FILE			*MU_FP;
static		char			MU_BUF[256];

//--------------------------------------------------------------------------//
// セーブ
bool	Gs_FileDataWrite(char *fn,char *src, int num)
{
	bool	ret = NG;

	if ((MU_FP = fopen(fn,"wb")) != NULL) {
		fwrite(src,1,num,MU_FP);
		fclose(MU_FP);
		ret	= OK;
    } 
//	else	System_Message(hWnd,"%sが書けないね",fn);
	return(ret);
}

// ファイルの中をクリア（ファイルがなければ作成する）
bool	Gs_ClearFILE(char *fn)
{
	bool	ret = NG;

	if ((MU_FP=fopen(fn,"wt"))!=NULL){
        fprintf(MU_FP,"\0");
        fclose(MU_FP);
		ret	= OK;
	}
	return(ret);
}

// テキスト形式でファイルへ書式指定出力（セーブ）
bool	Gs_TextWriteFILE(char *fn,char* Format,...)
{
	bool	ret = NG;

    vsprintf(MU_BUF,Format,(char*)(&Format+1));
    strcat(MU_BUF,"\n\0");
    if ((MU_FP=fopen(fn,"at"))!=NULL){
        fprintf(MU_FP,"%s",MU_BUF);
        fclose(MU_FP);
		ret	= OK;
    }
	return(ret);
}

//--------------------------------------------------------------------------//
// ロード
bool	Gs_FileDataLoad(char *fn,char *dst,int num)
{
	FILE	*fp;
	bool	ret = NG;

	if ((fp = fopen(fn,"rb")) != NULL) {
		fread(dst,1,num,fp);
		fclose(fp);
		ret	= OK;
    }
//	else	System_Message(hWnd,"%sが読めないね",fn);
	return(ret);
}

//--------------------------------------------------------------------------//
// 指定したファイルがあるかチェック
bool	Gs_FindFILE(char *fn)
{
	FILE	*fp;
	bool	ret = NG;
	if ((fp = fopen(fn,"rb")) != NULL) {
		fclose(fp);
		ret	= OK;
    }
	return(ret);
}

//--------------------------------------------------------------------------//
//　デバッグ ファイルへ出力
#define		DEB_FILENAME	"DEB_FILE.txt"

void	Gs_DebugFILEClear(void)
{
	Gs_ClearFILE(DEB_FILENAME);

}

void	Gs_DebugFILEOut(char* Format,...)
{
    vsprintf(MU_BUF,Format,(char*)(&Format+1));
    strcat(MU_BUF,"\n\0");
    if ((MU_FP=fopen(DEB_FILENAME,"at"))!=NULL){
        fprintf(MU_FP,"%s",MU_BUF);
        fclose(MU_FP);
    }
}

//―――――――――――――――――――――――――――――――――――――――――//
//
//					ゲームシーン管理
//
//      シーン関連初期化                     ：  SceneInit( )
//      現在のシーン番号の取得               ：  SceneMode( )
//      シーンが切り替わった直後かチェック   ：  SceneCheck( )
//      シーンを切り替える                   ：  SceneChange(シーン番号)
//      シーンを切り替える（Master)			 ：  SceneExport( )
//      一つ前のシーンへ戻る                 ：  SceneBack( )
//      一番初めのシーンへ                   ：  SceneStart( )
//
//―――――――――――――――――――――――――――――――――――――――――//
#include		"../Game.H"

static struct {
	BOOL	Flg;
	short	Draw;
	short	NowNo;
	short	OldNo;
	short	ChangeNo;
	short	KeepNo;
	bool	Exit;
	bool	Clear;
}Scene;

//----------------------------------------------------------------------------------//

//	シーン描画の許可
int		SceneDraw(int n)
{
	if (n>=0) {
		Scene.Draw = n;
	}
	return(Scene.Draw);
}

//	現在のシーン番号の取得
int		SceneMode(void)
{
	return(Scene.NowNo);
}

int		SceneOld(void)
{
	return(Scene.OldNo);
}

//	シーンが切り替わった直後か？
BOOL	SceneCheck(void)
{
	if (Scene.Flg) {
		Scene.Flg = OFF;
		Gs_FadeInit( );
		return	(OK);
	}
	return	(NG);
}

//　シーンを切り替える
void	SceneChange(int no)
{
	Scene.ChangeNo = no;
}

BOOL	SceneExport(void)
{
	bool	ret = OFF;

	if (Scene.ChangeNo != -1) {
		Scene.OldNo		= Scene.NowNo;
		Scene.NowNo		= Scene.ChangeNo;
		Scene.ChangeNo	= -1;
		Scene.Flg		= ON;
		ret				= ON;
	}
	return		ret;
}

//　一つ前のシーンへ戻る
void	SceneBack(void)
{
	SceneChange(Scene.OldNo);
}

//　一番初めのシーンへ
void	SceneStart(void)
{
	SceneChange(START_SCENE);
}

//　ウィンドウ終了制御
void	SceneExit(int x)
{
	if (x)		Scene.Exit = ON;
}

BOOL	SceneExitCheck(void)
{
	return(Scene.Exit);
}

// ゲーム画面クリア制御　ON=Clear:OFF=Keep
void	SceneClear(bool x)
{
	Scene.Clear = x;
}

BOOL	SceneClearCheck(void)
{
	return(Scene.Clear);
}


//　シーン関連初期化
void	SceneInit(void)
{
	Scene.OldNo		= 0;
	Scene.NowNo		= START_SCENE;
	Scene.ChangeNo	= -1;
	Scene.Flg		= ON;
	Scene.Draw		= ON;
	Scene.Exit		= OFF;
	Scene.Clear		= ON;
}


//======================================================================================//
//																						//
//								Ｄｉｒｅｃｔ　ＭＵＳＩＣ                                //
//																						//
//======================================================================================//
#include	<dmusici.h>

//音関係用
#define			MUSIC_MAX				16
#define			DEF_MASTERVOLUME		(-10*100)
#define			MAX_VOLUME				(20*100)
#define			MIN_VOLUME				(-100*100)


// オーディオパス　
enum {AP_BGM,AP_SE1,AP_SE2,AP_SE3,AP_SE4,AP_SE5,AP_SE6,AP_SEMAX,AP_MAX};


IDirectMusicPerformance8	*pPerformance;
IDirectMusicAudioPath8		*pAudioPath[AP_MAX];
IDirectMusicLoader8			*pLoader;
IDirectMusicSegment8		*pMusicData[MUSIC_MAX];
IDirectMusicSegmentState	*pMusicStt[MUSIC_MAX];

static	short				AP_Num[MUSIC_MAX];		// 使用しているオーディオパス
static	short				BGM_Num;
static	long				MasterVolume;			// マスター・ボリューム

/////////////////////////////////////////////////////////////////////////
//  DirectMusicの生成
/////////////////////////////////////////////////////////////////////////
HRESULT CreateDMusic(HWND hWnd)
{

	//comの初期化
	if(FAILED(CoInitialize(NULL)))	return E_FAIL;
	//パフォーマンスの生成
	if(FAILED(CoCreateInstance(CLSID_DirectMusicPerformance,NULL,
			CLSCTX_INPROC,IID_IDirectMusicPerformance8,
			(void**)&pPerformance)))
		return E_FAIL;
	//パフォーマンスの初期化
	if(FAILED(pPerformance->InitAudio(NULL,NULL,hWnd,
			DMUS_APATH_SHARED_STEREOPLUSREVERB,64,DMUS_AUDIOF_ALL,NULL)))
		return E_FAIL;

	for(int i=0;i<AP_MAX;i++){
		pAudioPath[i]					=NULL;
		if(FAILED(pPerformance->CreateStandardAudioPath(DMUS_APATH_DYNAMIC_STEREO,64,TRUE,&pAudioPath[i])))
			return E_FAIL;
	}
	//ローダーの作成
	if(FAILED(CoCreateInstance(CLSID_DirectMusicLoader,NULL,
			CLSCTX_INPROC,IID_IDirectMusicLoader8,(void**)&pLoader)))
		return E_FAIL;

	// マスタボリューム取得
	Gs_GetMasterVol( );

	// オーディオパスナンバー
	for (int i=0;i<MUSIC_MAX;i=i+1) {
		AP_Num[i] = -1;
	}

	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
//	WAVEファイルからセグメントにロード
/////////////////////////////////////////////////////////////////////////
HRESULT LoadWaveFile(WCHAR *fName,IDirectMusicSegment8 **ppSegment)
{
	if(FAILED(pLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
			IID_IDirectMusicSegment8,fName,(LPVOID*)ppSegment)))
		return E_FAIL;
	if(FAILED((*ppSegment)->Download(pPerformance)))
		return E_FAIL;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
//	MIDIファイルからセグメントにロード
/////////////////////////////////////////////////////////////////////////
HRESULT LoadMIDIFile(WCHAR *fName,IDirectMusicSegment8 **ppSegment)
{
	if(FAILED(pLoader->LoadObjectFromFile(CLSID_DirectMusicSegment,
			IID_IDirectMusicSegment8,fName,(LPVOID*)ppSegment)))
		return E_FAIL;
	//スタンダードMIDIファイルをDirectMusic用に変換
	if(FAILED((*ppSegment)->SetParam(GUID_StandardMIDIFile,
			0xFFFFFFFF,0,0,NULL)))
		return E_FAIL;
	if(FAILED((*ppSegment)->Download(pPerformance)))
		return E_FAIL;
	return S_OK;
}

/////////////////////////////////////////////////////////////////////////
//	セグメントのリリース
/////////////////////////////////////////////////////////////////////////
void ReleaseSegment(IDirectMusicSegment8 **ppSegment)
{
	if(pPerformance!=NULL){
		pPerformance->StopEx(*ppSegment,0,0);
	}
	if(*ppSegment!=NULL){
		(*ppSegment)->Release();
		*ppSegment=NULL;
	}
}

/////////////////////////////////////////////////////////////////////////
//	DirectXAudio関連のリリース
/////////////////////////////////////////////////////////////////////////
void ReleaseDMusic(void)
{
	if(pPerformance!=NULL){
		pPerformance->Stop(NULL,NULL,0,0);
		Sleep(10);
		pPerformance->CloseDown();
	}
	if(pLoader!=NULL){
		pLoader->Release();
		pLoader=NULL;
	}

	for(int i=0;i<AP_MAX;i++){
		if(pAudioPath[i]!=NULL){
			pAudioPath[i]->Release();
			pAudioPath[i]=NULL;
		}
	}

	if(pPerformance!=NULL){
		pPerformance->Release();
		pPerformance=NULL;
	}

	for (int i = 0;i<MUSIC_MAX;i++) {
		ReleaseSegment(&pMusicData[i]);
	}

	CoUninitialize();
}

/////////////////////////////////////////////////////////////////////////
//  セグメントを再生
/////////////////////////////////////////////////////////////////////////
void PlaySeg(IDirectMusicSegment8 *pSeg,int ap,int sec,IDirectMusicSegmentState *pStt)
{
	pPerformance->PlaySegmentEx(pSeg,
		NULL,NULL,
		DMUS_SEGF_SECONDARY * sec,
		0,
		&pStt,NULL,pAudioPath[ap]);
}

/////////////////////////////////////////////////////////////////////////
//  リピート設定
/////////////////////////////////////////////////////////////////////////
void SetRept(IDirectMusicSegment8 *pSeg)
{
	pSeg->SetRepeats(DMUS_SEG_REPEAT_INFINITE);
}

/////////////////////////////////////////////////////////////////////////
//  セグメントの再生停止
/////////////////////////////////////////////////////////////////////////
void StopSeg(IDirectMusicSegment8 *pSeg)
{
	pPerformance->StopEx(pSeg,0,0);
}

//======================================================================================//
//		Ｇｓ　ＬＩＢ　ＭＵＳＩＣ
//======================================================================================//
void	Gs_PlayBGM(GsSOUND no)
{

	BGM_Num		= no;
	PlaySeg(pMusicData[no],AP_BGM,0,pMusicStt[no]);
}

void	Gs_PlaySE(GsSOUND no)
{
	static int	pno=AP_SE1;

	if (pno++ >= AP_SEMAX)		pno = AP_SE1;
	AP_Num[no]	= pno;
	PlaySeg(pMusicData[no],pno,1,pMusicStt[no]);
}

void	Gs_PlaySE(GsSOUND no,int vol)
{
	static int	pno=AP_SE1;

	if (pno++ >= AP_SEMAX)		pno = AP_SE1;
	Gs_SetSEVol(pno,vol);
	AP_Num[no]	= pno;
	PlaySeg(pMusicData[no],pno,1,pMusicStt[no]);
}


void	Gs_StopMUSIC(GsSOUND no)
{
	AP_Num[no]	= -1;
	StopSeg(pMusicData[no]);
}

//  マスター音量取得
void	Gs_GetMasterVol(void)
{
	bool	ret;
	ret	= pPerformance->SetGlobalParam(GUID_PerfMasterVolume, (void*)&MasterVolume, sizeof(long));
    // マスタボリュームの取得に失敗
	if ( FAILED(ret) ) {
		MasterVolume	= DEF_MASTERVOLUME;
	}
}

//  マスター音量変更(-10000 ～ 1500の範囲）
short	Gs_SetMasterVol(int vol)
{
	bool	ret;
	if (vol > MAX_VOLUME)	vol = MAX_VOLUME;
	if (vol < MIN_VOLUME)	vol = MIN_VOLUME;

	ret = pPerformance->SetGlobalParam(GUID_PerfMasterVolume, (void*)&vol, sizeof(long)); 
	return(vol);
}

//　BGMの音量(-10000 ～ 1500の範囲）
short	Gs_SetBGMVol(int vol)
{
	bool	ret;
	if (vol > MAX_VOLUME)	vol = MAX_VOLUME;
	if (vol < MIN_VOLUME)	vol = MIN_VOLUME;

	ret = pAudioPath[AP_BGM]->SetVolume(vol, 0);  // ボリュームを即時変更
	return(vol);
}


//　SEの音量(-10000 ～ 2000の範囲）
short	Gs_SetSEVol(int no,int vol)
{
	bool	ret;
	if (vol > MAX_VOLUME)	vol = MAX_VOLUME;
	if (vol < MIN_VOLUME)	vol = MIN_VOLUME;

	ret = pAudioPath[no]->SetVolume(vol, 0);  // ボリュームを即時変更
	return(vol);
}

// BGMが鳴ってればTRUEを止まってればFALSEを返す
short	Gs_GetBGMInfo(void)
{
	short	ret	= false;
	if (pPerformance->IsPlaying(pMusicData[BGM_Num],pMusicStt[BGM_Num]) == S_OK)	ret = true;
	return	(ret);
}

//
void	Gs_ControlMUSIC(void)
{
}

//―――――――――――――――――――――――――――――――――――――――――//
//	ＳＯＵＮＤ　ＤＡＴＡ の破棄
//
//―――――――――――――――――――――――――――――――――――――――――//
void	Gs_ReleaseSOUND(GsSOUND no)
{
	ReleaseSegment(&pMusicData[no]);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	Wave の読み込み
//
//―――――――――――――――――――――――――――――――――――――――――//
GsSOUND	Gs_LoadWAVE(char *fname,bool rept)
{
	int		i = 0;
	WCHAR	Wfname[64];

	for (i=0;i<MUSIC_MAX;i++) {
		if (pMusicData[i] == NULL)		break;
	}

	MultiByteToWideChar(CP_ACP, 0, fname, -1, Wfname,sizeof(Wfname)/sizeof(Wfname[0]));

	if (FAILED(LoadWaveFile(Wfname,&pMusicData[i]))) {
		System_Message(GsHwnd,"ロードに失敗しました : %s",fname);
		i = 0;
	}
	else if (rept)	SetRept(pMusicData[i]);

	return(i);
}

//―――――――――――――――――――――――――――――――――――――――――//
//	Midi の読み込み
//
//―――――――――――――――――――――――――――――――――――――――――//
GsSOUND		Gs_LoadMIDI(char *fname,bool rept)
{
	int		i = 0;
	WCHAR	Wfname[64];

	for (i=0;i<MUSIC_MAX;i++) {
		if (pMusicData[i] == NULL)		break;
	}

	MultiByteToWideChar(CP_ACP, 0, fname, -1, Wfname,sizeof(Wfname)/sizeof(Wfname[0]));

	if (FAILED(LoadMIDIFile(Wfname,&pMusicData[i]))) {
		System_Message(GsHwnd,"ロードに失敗しました : %s",fname);
		i = 0;
	} 
	else if	(rept)	SetRept(pMusicData[i]);

	return(i);
}


//==========================================================================================================//
//								EOP																			//
//==========================================================================================================//
