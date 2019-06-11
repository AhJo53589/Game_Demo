#pragma once
#include "Util.h"
#include "DInput.h"
#include "Camera2drpg.h"
#include "Camera3dUse.h"
#include "Res.h"
#include "WorldMap.h"
#include "Player.h"
#include "UI.h"
#include "UIManager.h"

class Game2drpg
{
public:
	Game2drpg(HINSTANCE hInstance, TCHAR *wndCaption);
	virtual ~Game2drpg();

	HINSTANCE getInst() {return this->mhInstance;}
	HWND      getWnd() {return this->mhWnd;}

	virtual void onLostDevice() {}
	virtual void onResetDevice() {}
	virtual void update(float dt);
	virtual void draw();
	
	virtual Game2drpg &create();
	virtual void init();
	virtual void initWindow();
	virtual void initDirect3D();

	virtual int playing();
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	bool isDeviceLost();

	DInput* getInput() {return this->mInput;}
	Camera* getCamera() {return this->mCamera;}
	Camera2d* getUICamera() {return this->mUICamera;}
	Res* getRes() {return this->mRes;}
	Player* getPlayer() {return this->mPlayer;}
	UIManager *getUIManager() {return this->mUIManager;}

protected:
	TCHAR *mWndCaption;
	
	HINSTANCE mhInstance;
	HWND mhWnd;
	LPDIRECT3D9 mDirect3D;
	D3DPRESENT_PARAMETERS mParams;

	DInput *mInput;
	Camera *mCamera;
	Camera2d *mUICamera;
	Res *mRes;
	WorldMap *mWorldMap;
	Player *mPlayer;
	UIManager *mUIManager;
};

extern Game2drpg *game;
extern LPDIRECT3DDEVICE9 d3dDevice;
