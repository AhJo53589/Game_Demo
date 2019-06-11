#pragma once
#include "Util.h"
#include "Game2drpg.h"

extern Game2drpg *game;
extern LPDIRECT3DDEVICE9 d3dDevice;

//�ֱ���
#define RESOLUTIONW 800
#define RESOLUTIONH 600

//��Ƭ�߳�
#define TILESIDE 80.0f

//��ͼ��Ϣ
#define WORLDMAPWIDTH 100
#define WORLDMAPHEIGHT 100
extern int WorldMapData[WORLDMAPHEIGHT][WORLDMAPWIDTH];
//��������ö��
enum
{
	MT_UNKNOW = 0,
	MT_DESERT,
	MT_FOREST,
	MT_RIIVER,
	MT_CONST
};