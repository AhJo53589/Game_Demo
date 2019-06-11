#pragma once

#include "define.h"
#include "Game.h"

namespace pool
{
	namespace window
	{
		void CreateGameWindow(PGAME game, TCHAR *name, HINSTANCE hInstance = NULL, int nCmdShow = SW_SHOWNORMAL, HICON hIcon = NULL);

		bool InitializeWindow(PGAME game, TCHAR *name, HINSTANCE hInstance, int nCmdShow, HICON hIcon, HWND &lhWnd);

		HICON SethIcon(HICON hIcon);

		LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	}
}