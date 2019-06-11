#pragma once
#include "Util.h"
#include "Node.h"
#include "Camera2d.h"
#include "list"

//地图瓷片
class MapTile : public Node
{
public:
	MapTile() : mType(0), mAniTime(0.0f) {}
	virtual ~MapTile() {}

	virtual bool init(int x, int y);

	virtual void update(float dt);
	virtual void draw();
	//是否在可视区内
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

	//将一个矩形范围内的地图瓷片添加至工作队列
	void addTilesBy2Point(int x1, int y1, int x2, int y2);
	//当前工作队列为空时使用全部填充的方式添加瓷片
	void addTilesAll(int x, int y);
	//当前工作队列不为空时，所需添加的内容是一个曲尺型，需要更具4种情况，来添加2个矩形范围
	void addTilesPart(int x, int y);
	//删除不在可视区内的瓷片
	void delOutTiles(int x, int y);

};

