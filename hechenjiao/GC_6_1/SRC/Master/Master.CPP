//======================================================================================//
//																						//
//																						//
//							Entry Program												//
//																						//
//																						//
//======================================================================================//
#define			WINVER			0x0501
#define			_WIN32_WINNT	0x0501
#include		<windows.h>
#include		<winuser.h>
#include		"Ci-Lib.H"
#include		"../Game.H"

//======================================================================================//
//																						//
//							Game MAIN  Program											//
//																						//
//======================================================================================//
extern		SCENE_TBL	*SceneTbl[SCENE_MAX];
extern		void		GameAllInit(void);
extern		void		GameAllLoad(void);
extern		void		GameAllExit(void);

extern		void		FullKEYInfo(void);

//--------------------------------------------------------------------------//
//	●	Game Main Loop														//
//--------------------------------------------------------------------------//
BOOL	GameMainLoop(void)
{
	bool	Ret	= OK;
	static short	LoadID;


	if(SyncWait(FRAME_LATE)){

		// 入力情報セット
		Gs_CheckPAD( );
		Gs_CheckKEY( );
		FullKEYInfo( );

		// MOUSE INFO SET
		Mouse.Dx	= Mouse.Px - Mouse.Ox;
		Mouse.Dy	= Mouse.Py - Mouse.Oy;

		// シーン切り替え後　初期化
		if (SceneCheck( ))	{
			// Load
			LoadID = (*(SceneTbl + SceneMode( )))->Load ( );
			// Init
			(*(SceneTbl + SceneMode( )))->Init ( );
		}

		DEB_INIT	( );

		//	シーンの内部処理
		(*(SceneTbl + SceneMode( )))->Task ( );

		// シーン切り替え処理
		if (SceneExport( ))		(*(SceneTbl + SceneOld( )))->Exit ( );
		else {


			//	シーンの描画
			if (SceneDraw(-1)) {

				Gs_DrawClear( );
				Gs_DrawScene( );

				Gs_Begin2D	( );
				(*(SceneTbl + SceneMode( )))->Draw ( );	
				Gs_End2D	( );

				// デバッグ用
				Gs_Begin2D	( );
				Gs_FadeDraw ( );
				DEB_DRAW	( );
				Gs_End2D	( );

				Gs_DrawPresent( );
			}
		}

		// MOUSE AFTER SET
		Mouse.Trg	= NULL;
		Mouse.Pul	= NULL;
		Mouse.WClk	= NULL;
		Mouse.Ox	= Mouse.Px;
		Mouse.Oy	= Mouse.Py;
		Mouse.Scr	= NULL;
	}


	// Always ESC Key  Exit
	if (Gs_GetVKEY(ESC) || SceneExitCheck( ))		{	Ret = NG;	}

	return	(Ret);
}

//--------------------------------------------------------------------------//
//	●	Game Main Init														//
//--------------------------------------------------------------------------//
BOOL	GameMainInit(HWND hWnd)
{

	if (!Gs_Init(hWnd))			return(NG);

	SceneInit	( );

	GameAllInit	( );
	GameAllLoad	( );

	// KEY
	FullKEY.Now		= 0;
	FullKEY.Old 	= 0;
	FullKEY.Trg 	= 0;
	FullKEY.Pul 	= 0;

	return	(OK);
}

//--------------------------------------------------------------------------//
//	●	Game Main Exit														//
//--------------------------------------------------------------------------//
BOOL	GameMainExit(void)
{

	GameAllExit	( );
	Gs_Exit		( );

	return (OK);
}

//======================================================================================//
//																						//
//							Windows MAIN  Program										//
//																						//
//======================================================================================//
HWND			hWnd;
HINSTANCE		hInst;
MS_PRIM			Mouse;
WIN_PRIM		mWin;

//--------------------------------------------------------------------------//
//	●	Windows Proc （メッセージ処理部）                                   //
//--------------------------------------------------------------------------//
LRESULT WINAPI WndProc(HWND hWnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	PAINTSTRUCT			ps;
	static MMRESULT		mmRes;

	switch(msg)
	{
		case WM_CREATE:					//ウィンドウ生成メッセージ
			mmRes=timeBeginPeriod(1); 
			break;

		case WM_PAINT:					//再描画要求メッセージ
			BeginPaint(hWnd,&ps);
			EndPaint(hWnd,&ps);
			break;

		case WM_DESTROY:				//ウィンドウの破棄メッセージ
			mmRes=timeEndPeriod(mmRes);
			PostQuitMessage(0);
			break;

		// MOUSE
		case WM_MOUSEMOVE:
			Mouse.Px	=	LOWORD(lParam);
			Mouse.Py	=	HIWORD(lParam);

			//if (!MsAREA(0,0,WINW,WINH))	{
			//	Mouse.Now = 0;
			//}
			break;

		case WM_LBUTTONDOWN:
			Mouse.Old = Mouse.Now;
			Mouse.Now |= _lMOUSE;
			Mouse.Trg |= _lMOUSE;
			Mouse.Px	=LOWORD(lParam);
			Mouse.Py	=HIWORD(lParam); 
			break;
		case WM_LBUTTONUP:
			Mouse.Now &= ~_lMOUSE;
			Mouse.Pul |=  _lMOUSE;
			break;
		case WM_RBUTTONDOWN:
			Mouse.Now |= _rMOUSE;
			Mouse.Trg |= _rMOUSE;
			Mouse.Px	=LOWORD(lParam );
			Mouse.Py	=HIWORD(lParam ); 
			break;
		case WM_RBUTTONUP:
			Mouse.Now &= ~_rMOUSE;
			Mouse.Pul |=  _rMOUSE;
			break;
		case WM_LBUTTONDBLCLK:
			Mouse.WClk |= _lMOUSE;
			break;
		case WM_RBUTTONDBLCLK:
			Mouse.WClk |= _rMOUSE;
			break;
		//	マウスホイール追加
		case WM_MOUSEWHEEL:
			Mouse.Scr = (short)HIWORD(wParam)/MS_WHEEL_UNIT;
			break;

		case WM_MOVE:
			mWin.Sx =(int)LOWORD(lParam);
			mWin.Sy =(int)HIWORD(lParam);
			break;


		default:						//デフォルトのメッセージ
			return DefWindowProc(hWnd,msg,wParam,lParam);
	}

	return 0L; 
}

//--------------------------------------------------------------------------//
//	●	Win Main （PeekMessage使用）										//
//--------------------------------------------------------------------------//
INT WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,	LPSTR lpCmdLine,int nShowCmd)
{
	#define			CLASS_NAME		"GsWindow"
	WNDCLASSEX		wc;
	MSG				msg;
	DWORD			style;
	int				sx,sy,w,h;

	// 重複 Window　禁止
	if (FindWindow(CLASS_NAME,TITLE_NAME) != NULL)		return 0;

	//ウィンドウの形式
	wc.hInstance		= hInstance;
	wc.lpszClassName	= CLASS_NAME;
	wc.lpfnWndProc		= WndProc;
	wc.style			= CS_CLASSDC | CS_DBLCLKS;
	wc.cbSize			= sizeof(WNDCLASSEX);
	wc.lpszMenuName		= NULL;
	wc.hIcon			= LoadIcon(hInstance,"GC_ICON");
	wc.hIconSm			= LoadIcon(hInstance,"GC_ICON");
	wc.hCursor			= LoadCursor(NULL,IDC_ARROW);
	wc.cbClsExtra		= 0L;
	wc.cbWndExtra		= 0L;
	wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH);

	//ウィンドウの登録
	RegisterClassEx(&wc);

	//ウィンドウの生成
	int		SizeX	= WINW;
	int		SizeY	= WINH;

	#ifdef FULLSCREEN
		style	= WS_DLGFRAME|WS_POPUP;//|WS_BORDER;
//		style	= WS_POPUP;
//		style	= WS_THICKFRAME|WS_POPUP;//|WS_BORDER;
		sx		= (GetSystemMetrics(SM_CXSCREEN)-SizeX)/2;
		sy		= (GetSystemMetrics(SM_CYSCREEN)-SizeY)/2;
		w		= SizeX;
		h		= SizeY;

		sx		= (GetSystemMetrics(SM_CXSCREEN)-(SizeX+GetSystemMetrics(SM_CXBORDER)+GetSystemMetrics(SM_CXEDGE)+
					GetSystemMetrics(SM_CXDLGFRAME)))/2;
		sy		= (GetSystemMetrics(SM_CYSCREEN)-(SizeY+GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYBORDER)+
					GetSystemMetrics(SM_CYDLGFRAME)))/2;
		w		= SizeX+GetSystemMetrics(SM_CXBORDER)+GetSystemMetrics(SM_CXEDGE)+GetSystemMetrics(SM_CXDLGFRAME);
		h		= SizeY+GetSystemMetrics(SM_CYBORDER)+GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYDLGFRAME);

	#else if 
		style	= WS_OVERLAPPED|WS_CAPTION|WS_SYSMENU|WS_BORDER|WS_MINIMIZEBOX;
		sx		= (GetSystemMetrics(SM_CXSCREEN)-(SizeX+GetSystemMetrics(SM_CXBORDER)+GetSystemMetrics(SM_CXEDGE)+
					GetSystemMetrics(SM_CXDLGFRAME)))/2;
		sy		= (GetSystemMetrics(SM_CYSCREEN)-(SizeY+GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYBORDER)+
					GetSystemMetrics(SM_CYDLGFRAME)))/2-GetSystemMetrics(SM_CYCAPTION);
		w		= SizeX+GetSystemMetrics(SM_CXBORDER)+GetSystemMetrics(SM_CXEDGE)+GetSystemMetrics(SM_CXDLGFRAME);
		h		= SizeY+GetSystemMetrics(SM_CYCAPTION)+GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYBORDER)+GetSystemMetrics(SM_CYDLGFRAME);
	#endif

	hWnd=CreateWindowEx(
			WS_EX_APPWINDOW,
			CLASS_NAME,
			TITLE_NAME,
			style,
			sx,sy,w,h,
			NULL,NULL,hInstance,NULL);


//	mWin.DispX	= (GetSystemMetrics(SM_CXBORDER)+GetSystemMetrics(SM_CXEDGE)+GetSystemMetrics(SM_CXDLGFRAME))/2;
//	mWin.DispY	= GetSystemMetrics(SM_CYCAPTION)+(GetSystemMetrics(SM_CYEDGE)+GetSystemMetrics(SM_CYBORDER)+GetSystemMetrics(SM_CYDLGFRAME))/2;
	mWin.DispX	= GetSystemMetrics(SM_CXSCREEN)/2-WINW/2;
	mWin.DispY	= GetSystemMetrics(SM_CYSCREEN)/2-WINH/2;

	// 環境　ゲームの初期化
	hInst = hInstance;

	if (!GameMainInit( hWnd )) {
		GameMainExit( );
		return	(0);
	}

#ifndef MOUSE_DISP
	ShowCursor(OFF);
#endif
	//ウィンドウの表示
	ShowWindow(hWnd,SW_SHOWDEFAULT);
	UpdateWindow(hWnd);

	ZeroMemory(&msg,sizeof(msg));

	//メッセージループ
	while(msg.message!=WM_QUIT){
		//ウィンドウメッセージの処理
		if(PeekMessage(&msg,NULL,0L,0L,PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		//ゲームループ
		else {

			if(!GameMainLoop( )) {
				DestroyWindow(hWnd);
			}
		}
		Sleep(1);
	}
	// ゲーム　環境終了処理
	GameMainExit( );

	return	(0);
}


//----------------------------------------------//
// Non Operation FUNCTION						//
//----------------------------------------------//
void	_NOP	(void)	{ }
bool	_NOPR	(void)	{return 0;}


//======================================================================================//
//							EOP															//
//======================================================================================//
