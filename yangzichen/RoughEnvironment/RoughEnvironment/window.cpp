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
				::MessageBox(NULL, TEXT("��������ʧ��"), TEXT("CreateGameWindow"), MB_OK);
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
			//����������������Ĵ��ھ��еķ��
			wndClass.style = CS_DBLCLKS;
			//���ڴ�������ָ��
			wndClass.lpfnWndProc = WndProc;
			//ָ�������ڴ�����ṹ��ĸ����ֽ���
			wndClass.cbClsExtra = 0;
			//ָ�������ڴ���������ĸ����ֽ���
			wndClass.cbWndExtra = 0;
			//��ģ����������
			wndClass.hInstance = hInstance;
			//ͼ��ľ��
			wndClass.hIcon = SethIcon(hIcon);
			//���ľ��
			wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			//������ˢ�ľ��
			wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			//ָ��˵���ָ��
			wndClass.lpszMenuName = NULL;
			//ָ�������Ƶ�ָ��
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