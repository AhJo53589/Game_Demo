#include "window.h"

namespace pool
{
	namespace window
	{
		bool CreatedWindow = false;
		
		PGAME Game = NULL;

		void CreateGameWindow(PGAME game, TCHAR *name, HINSTANCE hInstance, int nCmdShow, HICON hIcon)
		{
			if( hInstance == NULL )
				hInstance = (HINSTANCE)GetModuleHandle(NULL);

			Game = game;

			HWND hWnd;

			if (!InitializeWindow(game, name, hInstance, nCmdShow, hIcon, hWnd))
			{
				::MessageBox(NULL, TEXT("创建窗口失败"), TEXT("CreateGameWindow"), MB_OK);
					return;
			}

			if (!(game->Initialize(hWnd, hInstance)))
				return;

			MSG msg;
			msg.message = WM_NULL;
			PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

			while( WM_QUIT != msg.message )
			{

				if(PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE ))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else
				{
					game->Loop();
				}
			}

			return;
		}
		
		bool InitializeWindow(PGAME game, TCHAR *name, HINSTANCE hInstance, int nCmdShow, HICON hIcon, HWND &lhWnd)
		{
			if (CreatedWindow)
				return true;

			WNDCLASS wndClass;
			//从这个窗口类派生的窗口具有的风格
			wndClass.style = CS_DBLCLKS;
			//窗口处理函数的指针
			wndClass.lpfnWndProc = WndProc;
			//指定紧跟在窗口类结构后的附加字节数
			wndClass.cbClsExtra = 0;
			//指定紧跟在窗口事例后的附加字节数
			wndClass.cbWndExtra = 0;
			//本模块的事例句柄
			wndClass.hInstance = hInstance;
			//图标的句柄
			wndClass.hIcon = SethIcon(hIcon);
			//光标的句柄
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			//背景画刷的句柄
			wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			//指向菜单的指针
			wndClass.lpszMenuName = NULL;
			//指向类名称的指针
			wndClass.lpszClassName = TEXT("DxWindowClass");

			RegisterClass(&wndClass);

			HWND hWnd;
			if (game->mFullscreen)
			{
				hWnd = CreateWindow(TEXT("DxWindowClass"), name, WS_POPUP | WS_SYSMENU | WS_VISIBLE,
					0, 0, game->mWidth, game->mHeight, 0, NULL, hInstance, NULL);
			}
			else
			{
				RECT rc;
				SetRect(&rc, 0, 0, game->mWidth, game->mHeight);
				AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, false);

				hWnd = CreateWindow(TEXT("DxWindowClass"), name, WS_OVERLAPPEDWINDOW,
					0, 0, (rc.right - rc.left), (rc.bottom - rc.top), 0, NULL, hInstance, 0);
			}

			if (!hWnd)
				return false;
			lhWnd = hWnd;

			CreatedWindow = true;

			ShowWindow(hWnd, nCmdShow);
			UpdateWindow(hWnd);
			return true;
		}

		HICON SethIcon(HICON hIcon)
		{
			if( hIcon == NULL ) 
				return LoadIcon(NULL, IDI_APPLICATION);
			else
				return hIcon;
		}

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			 switch(uMsg)
			{
			case WM_SIZE:
				Game->mWidth = LOWORD(lParam);
				Game->mHeight = HIWORD(lParam);
				break;
			case WM_DESTROY:
			case WM_CLOSE:
				PostQuitMessage(0);
				return 0;
				break;
			}
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
}