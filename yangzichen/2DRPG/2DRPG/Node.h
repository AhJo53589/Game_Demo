#pragma once
#include "Util.h"

class Node;

//空间变化的监听者
class MovementEventListener
{
public:
	virtual void TranslationEvent(Node *n, D3DXVECTOR3 t) = 0;
};

//空间节点类 用于空间变化的矩阵运算
class Node : public GameCell
{
public:
	Node();
	virtual ~Node() {};

	//移动至
	void at(float x, float y, float z);
	//移动
	void move(float x, float y, float z);

	//旋转至
	void rotation(D3DXVECTOR3 axis, float angle);
	void rotation(D3DXMATRIX matrix);
	//旋转
	void turn(D3DXVECTOR3 axis, float angle);
	void turn(D3DXMATRIX matrix);

	//缩放至
	void scaling(float x, float y, float z);
	//缩放
	void size(float x, float y, float z);

	D3DXMATRIX getRotationMatrix();
	D3DXMATRIX getMoveMatrix();
	D3DXMATRIX getSizeMatrix();

	D3DXVECTOR3 getMoveVec3() {return this->mT;}
	D3DXVECTOR3 getSizeVec3() {return this->mS;}

	virtual D3DXMATRIX getMatrix();

	Node* mPatent;

	void addListener(MovementEventListener *l);
    void removeListener(MovementEventListener *l);

protected:
	D3DXMATRIX mRotationMatrix;
	D3DXVECTOR3 mT, mS;

	std::vector<MovementEventListener *> mListenerList;

	void notifyTranslationEvent(D3DXVECTOR3 t);
};

