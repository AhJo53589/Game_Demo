#include "WorldMap.h"
#include "Define2DRPG.h"

//地图起点(左下角) 
#define MAPBASEPOINT_X (- (TILESIDE * WORLDMAPWIDTH  / 2))
#define MAPBASEPOINT_Y (- (TILESIDE * WORLDMAPHEIGHT / 2))
//分配瓷片 行数 列数 总数 
#define MAP_TILE_XNUM (int)(RESOLUTIONW/TILESIDE + 2)
#define MAP_TILE_YNUM (int)(RESOLUTIONH/TILESIDE + 2)
#define MAXTILES (MAP_TILE_XNUM * MAP_TILE_YNUM)

bool MapTile::init(int x, int y)
{
	if (x < 0 || y < 0 || x >= WORLDMAPWIDTH || y >= WORLDMAPHEIGHT)
	{
		this->mType = MT_UNKNOW;
		this->mMapx = -1;
		this->mMapy = -1;
		return false;
	}
	else
	{
		this->mType = WorldMapData[y][x];
		if (this->mType == MT_RIIVER)
			this->mAniTime = (float)(rand() % 3);
		else
			this->mAniTime = 0.0f;
		this->at(
			MAPBASEPOINT_X + TILESIDE * x + TILESIDE / 2,
			MAPBASEPOINT_Y + TILESIDE * y + TILESIDE / 2,
			400.0f);
		this->mMapx = x;
		this->mMapy = y;
		return true;
	}
}

void MapTile::update(float dt)
{
	if (this->mType != MT_RIIVER)
		return;

	this->mAniTime += dt * 2;
	if (dt > 3.0f)
	{
		dt = 0.0f;
	}
}

void MapTile::draw()
{
	switch (this->mType)
	{
	case MT_DESERT:
		game->getRes()->getModel()->draw(MAP_0, this->getMatrix(), game->getCamera());
		break;
	case MT_FOREST:
		game->getRes()->getModel()->draw(MAP_1, this->getMatrix(), game->getCamera());
		break;
	case MT_RIIVER:
		int tex = MAP_2_0;
		tex += (int)this->mAniTime % 3;
		game->getRes()->getModel()->draw(tex, this->getMatrix(), game->getCamera());
		break;
	}
}

#define MAP_W_OUTLINE (RESOLUTIONW + TILESIDE) / 2
#define MAP_H_OUTLINE (RESOLUTIONH + TILESIDE) / 2

bool MapTile::isOutCamera(int x, int y)
{
	if (this->mMapx < 0 || this->mMapy < 0 ||
		this->mMapx < x || this->mMapy < y ||
		this->mMapx >= x + MAP_TILE_XNUM || this->mMapy >= y + MAP_TILE_YNUM)
		return true;
	else
		return false;
}

WorldMap::WorldMap()
{
	MapTile *t;
	for (int n=0 ; n!=MAXTILES ; ++n)
	{
		t = new MapTile();
		t->mPatent = this;
		this->mTileFreeList.push_back(t);
	}
}

WorldMap::~WorldMap()
{
	for (std::list <MapTile *> ::iterator it = this->mTileUsingList.begin(); it != this->mTileUsingList.end(); ++ it)
	{
		Delete((*it));
	}

	for (std::list <MapTile *> ::iterator it = this->mTileFreeList.begin(); it != this->mTileFreeList.end(); ++ it)
	{
		Delete((*it));
	}
}

void WorldMap::init()
{
	D3DXVECTOR3 v3 = game->getCamera()->getPos();
	this->clearUsingTiles();

	int x = (int)((v3[0] - MAPBASEPOINT_X - RESOLUTIONW/2) / TILESIDE);
	int y = (int)((v3[1] - MAPBASEPOINT_Y - RESOLUTIONH/2) / TILESIDE);
	this->addTilesAll(x, y);
	this->mMapx = x;
	this->mMapy = y;
}

void WorldMap::clearUsingTiles()
{
	MapTile *t;
	while (!this->mTileUsingList.empty())
	{
		t = this->mTileUsingList.back();
		this->mTileUsingList.pop_back();
		this->mTileFreeList.push_back(t);
	}
}

MapTile *WorldMap::getTile()
{
	if (this->mTileFreeList.empty())
	{
		printf("empty!!!!\n");
		return NULL;
	}
	else
	{
		MapTile *t = this->mTileFreeList.back();
		this->mTileFreeList.pop_back();
		this->mTileUsingList.push_back(t);

		return t;
	}
}

void WorldMap::addTilesBy2Point(int x1, int y1, int x2, int y2)
{
	if (x1 == x2 || y1 == y2)
		return;

	if (x1 < 0)
		x1 = 0;
	if (y1 < 0)
		y1 = 0;
	if (x2 > WORLDMAPWIDTH)
		x2 = WORLDMAPWIDTH;
	if (y2 > WORLDMAPHEIGHT)
		y2 = WORLDMAPHEIGHT;

	MapTile *t;
	for (int n=y1 ; n<y2 ; ++n)
	{
		for (int m=x1 ; m<x2; ++m)
		{
			t = this->getTile();
			if (t)
			{
				t->init(m, n);
			}
		}
	}
}

void WorldMap::addTilesAll(int x, int y)
{
	int x2 = x + MAP_TILE_XNUM, y2 = y + MAP_TILE_YNUM;
	this->addTilesBy2Point(x ,y, x2, y2);
}

void WorldMap::addTilesPart(int x, int y)
{
	int x1, y1, x2, y2;

	if (x >= this->mMapx && y >= this->mMapy)
	{
		x1 = x;
		y1 = this->mMapy + MAP_TILE_YNUM;
		x2 = x + MAP_TILE_XNUM;
		y2 = y + MAP_TILE_YNUM;
		this->addTilesBy2Point(x1 ,y1, x2, y2);

		x1 = this->mMapx + MAP_TILE_XNUM;
		y1 = y;
		x2 = x + MAP_TILE_XNUM;
		y2 = this->mMapy + MAP_TILE_YNUM;
		this->addTilesBy2Point(x1 ,y1, x2, y2);
	}
	else if (x >= this->mMapx && y < this->mMapy)
	{
		x1 = x;
		y1 = y;
		x2 = x + MAP_TILE_XNUM;
		y2 = this->mMapy;
		this->addTilesBy2Point(x1 ,y1, x2, y2);

		x1 = this->mMapx + MAP_TILE_XNUM;
		y1 = this->mMapy;
		x2 = x + MAP_TILE_XNUM;
		y2 = y + MAP_TILE_YNUM;
		this->addTilesBy2Point(x1 ,y1, x2, y2);
	}
	else if (x < this->mMapx && y >= this->mMapy)
	{
		x1 = x;
		y1 = this->mMapy + MAP_TILE_YNUM;
		x2 = x + MAP_TILE_XNUM;
		y2 = y + MAP_TILE_YNUM;
		this->addTilesBy2Point(x1 ,y1, x2, y2);

		x1 = x;
		y1 = y;
		x2 = this->mMapx;
		y2 = this->mMapy + MAP_TILE_YNUM;
		this->addTilesBy2Point(x1 ,y1, x2, y2);
	}
	else if (x < this->mMapx && y < this->mMapy)
	{
		x1 = x;
		y1 = y;
		x2 = x + MAP_TILE_XNUM;
		y2 = this->mMapy;
		this->addTilesBy2Point(x1 ,y1, x2, y2);

		x1 = x;
		y1 = this->mMapy;
		x2 = this->mMapx;
		y2 = y + MAP_TILE_YNUM;
		this->addTilesBy2Point(x1 ,y1, x2, y2);
	}
}

void WorldMap::delOutTiles(int x, int y)
{
	int a = 0;
	std::list <MapTile *> ::iterator it = this->mTileUsingList.begin();
	std::list <MapTile *> ::iterator temp;
	while(it != this->mTileUsingList.end())
	{
		if ((*it)->isOutCamera(x, y))
		{
			temp = it;
			++it;
			this->mTileFreeList.push_back(*temp);
			this->mTileUsingList.erase(temp);
			++a;
		}
		else
		{
			++it;
		}
	}
}

void WorldMap::changeViewCenter(float centerx, float centery)
{
	int x = (int)((centerx - MAPBASEPOINT_X - RESOLUTIONW/2) / TILESIDE);
	int y = (int)((centery - MAPBASEPOINT_Y - RESOLUTIONH/2) / TILESIDE);

	if (this->mMapx == x && this->mMapy == y)
		return;

	this->delOutTiles(x, y);
	if (this->mTileUsingList.empty())
		this->addTilesAll(x, y);
	else
		this->addTilesPart(x, y);

	this->mMapx = x;
	this->mMapy = y;
}

void WorldMap::update(float dt)
{
	for (std::list <MapTile *> ::iterator it = this->mTileUsingList.begin(); it != this->mTileUsingList.end(); ++ it)
	{
		(*it)->update(dt);
	}
}

void WorldMap::draw()
{
	for (std::list <MapTile *> ::iterator it = this->mTileUsingList.begin(); it != this->mTileUsingList.end(); ++ it)
	{
		(*it)->draw();
	}
}

void WorldMap::CameraMoveEvent(Camera *c, D3DXVECTOR3 pos)
{
	this->changeViewCenter(pos[0], pos[1]);
}
