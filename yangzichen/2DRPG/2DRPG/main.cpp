#include "Game2drpg.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance, PSTR cmdLine, int showCmd)
{
	Game2drpg game2drpg(hInstance, TEXT("2d RPG"));
	game = &game2drpg.create();
	 
	game->init();
	game->playing();
	return 0;
}