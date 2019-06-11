#pragma once
#include "Util.h"
#include "Game2drpg.h"

extern Game2drpg *game;
extern LPDIRECT3DDEVICE9 d3dDevice;

//分辨率
#define RESOLUTIONW 800
#define RESOLUTIONH 600

//瓷片边长
#define TILESIDE 80.0f

//地图信息
#define WORLDMAPWIDTH 100
#define WORLDMAPHEIGHT 100
extern int WorldMapData[WORLDMAPHEIGHT][WORLDMAPWIDTH];
//地形种类枚举
enum
{
	MT_UNKNOW = 0,
	MT_DESERT,
	MT_FOREST,
	MT_RIIVER,
	MT_CONST
};