#include "main.h"
#include "RoughEnvironment.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	pool::CGamePool game;
	game.SetCamera(new pool::CCameraThirdPerson());
	game.SetNode(new CWorld());
	//game.mFullscreen = TRUE;
	pool::window::CreateGameWindow(&game, TEXT("RoughEnviroument"), hInstance, nCmdShow);
}