#include "Player.h"
#include "Res.h"
#include "Define2DRPG.h"
#include "Camera3dUse.h"

#define PLAYER_MAPSIZE_L (- (TILESIDE * WORLDMAPWIDTH / 2 - TILESIDE / 2))
#define PLAYER_MAPSIZE_R (  (TILESIDE * WORLDMAPWIDTH / 2 - TILESIDE / 2))
#define PLAYER_MAPSIZE_U (- (TILESIDE * WORLDMAPHEIGHT / 2 - TILESIDE / 2))
#define PLAYER_MAPSIZE_D (  (TILESIDE * WORLDMAPHEIGHT / 2 - TILESIDE / 2))

void Player::InputEvent(DInput *n, float dt)
{
	float speed = TILESIDE * dt * 10;
	D3DXVECTOR3 v3 = this->mT;;
	bool flag = false;
	if (n->keyDown(DIK_RIGHT))
	{
		this->rotation(D3DXVECTOR3(0.0f, 0.0f, -1.0f), 90 * oC);
		v3[0] += speed;
		flag = true;
	}
	else if (n->keyDown(DIK_LEFT))
	{
		this->rotation(D3DXVECTOR3(0.0f, 0.0f, -1.0f), 270 * oC);
		v3[0] += -speed;
		flag = true;
	}
	else if(n->keyDown(DIK_UP))
	{
		this->rotation(D3DXVECTOR3(0.0f, 0.0f, -1.0f), 0 * oC);
		v3[1] += speed;
		flag = true;
	}
	else if (n->keyDown(DIK_DOWN))
	{
		this->rotation(D3DXVECTOR3(0.0f, 0.0f, -1.0f), 180 * oC);
		v3[1] += -speed;
		flag = true;
	}

	if (flag)
	{
		if (v3[0] < PLAYER_MAPSIZE_L)
			v3[0] = PLAYER_MAPSIZE_L;
		if (v3[0] > PLAYER_MAPSIZE_R)
			v3[0] = PLAYER_MAPSIZE_R;
		if (v3[1] < PLAYER_MAPSIZE_U)
			v3[1] = PLAYER_MAPSIZE_U;
		if (v3[1] > PLAYER_MAPSIZE_D)
			v3[1] = PLAYER_MAPSIZE_D;

		this->at(v3[0], v3[1], v3[2]);
	}

#if defined(TEST3D)
	Camera3dUse *tmpCamera = dynamic_cast<Camera3dUse *>(game->getCamera());
	if (n->keyDown(DIK_O))
	{
		tmpCamera->setCameraFree(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(this->mT[0], this->mT[1], this->mT[2]-500));
	}
	else if (n->keyDown(DIK_P))
	{
		tmpCamera->setCameraLock(D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0), D3DXVECTOR3(this->mT[0], this->mT[1], this->mT[2]), 500);
	}
	else if (n->keyDown(DIK_K))
	{
		float tmp = tmpCamera->getDistance();
		if (tmp!=0)
		{
			tmp += 50 * dt;
			tmp = (tmp > 700) ? 700 : tmp;
			tmpCamera->setDistance(tmp);
		}
	}
	else if (n->keyDown(DIK_L))
	{
		float tmp = tmpCamera->getDistance();
		if (tmp!=0)
		{
			tmp -= 50 * dt;
			tmp = (tmp < 300) ? 300 : tmp;
			tmpCamera->setDistance(tmp);
		}
	}
	else if (n->keyDown(DIK_A))
	{
		tmpCamera->trunX(30.0f * dt);
	}
	else if (n->keyDown(DIK_S))
	{
		tmpCamera->trunY(30.0f * dt);
	}
	else if (n->keyDown(DIK_D))
	{
		tmpCamera->trunZ(30.0f * dt);
	}
	else if (n->keyDown(DIK_Q))
	{
		tmpCamera->trunX(-30.0f * dt);
	}
	else if (n->keyDown(DIK_W))
	{
		tmpCamera->trunY(-30.0f * dt);
	}
	else if (n->keyDown(DIK_E))
	{
		tmpCamera->trunZ(-30.0f * dt);
	}
#endif
}

void Player::update(float dt)
{
}

void Player::draw()
{
	game->getRes()->getModel()->draw(PLAYER, this->getMatrix(), game->getCamera());
}