#pragma once
#include "Util.h"
#include "Node.h"
#include "Camera2d.h"
#include "list"

//��ͼ��Ƭ
class MapTile : public Node
{
public:
	MapTile() : mType(0), mAniTime(0.0f) {}
	virtual ~MapTile() {}

	virtual bool init(int x, int y);

	virtual void update(float dt);
	virtual void draw();
	//�Ƿ��ڿ�������
	bool isOutCamera(int x, int y);

protected:
	int mType;
	float mAniTime;
	int mMapx, mMapy;
};

class WorldMap : public Node, public CameraMoveEventListener
{
public:
	WorldMap();
	virtual ~WorldMap();

	virtual void init();

	virtual void update(float dt);
	virtual void draw();

	void clearUsingTiles();
	void changeViewCenter(float centerx, float centery);
	void CameraMoveEvent(Camera *c, D3DXVECTOR3 pos);

protected:
	std::list<MapTile *> mTileUsingList, mTileFreeList;
	int mMapx, mMapy;

	MapTile * getTile();

	//��һ�����η�Χ�ڵĵ�ͼ��Ƭ�������������
	void addTilesBy2Point(int x1, int y1, int x2, int y2);
	//��ǰ��������Ϊ��ʱʹ��ȫ�����ķ�ʽ��Ӵ�Ƭ
	void addTilesAll(int x, int y);
	//��ǰ�������в�Ϊ��ʱ��������ӵ�������һ�������ͣ���Ҫ����4������������2�����η�Χ
	void addTilesPart(int x, int y);
	//ɾ�����ڿ������ڵĴ�Ƭ
	void delOutTiles(int x, int y);

};

