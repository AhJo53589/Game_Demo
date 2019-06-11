#pragma once
#include "Util.h"

class Node;

//�ռ�仯�ļ�����
class MovementEventListener
{
public:
	virtual void TranslationEvent(Node *n, D3DXVECTOR3 t) = 0;
};

//�ռ�ڵ��� ���ڿռ�仯�ľ�������
class Node : public GameCell
{
public:
	Node();
	virtual ~Node() {};

	//�ƶ���
	void at(float x, float y, float z);
	//�ƶ�
	void move(float x, float y, float z);

	//��ת��
	void rotation(D3DXVECTOR3 axis, float angle);
	void rotation(D3DXMATRIX matrix);
	//��ת
	void turn(D3DXVECTOR3 axis, float angle);
	void turn(D3DXMATRIX matrix);

	//������
	void scaling(float x, float y, float z);
	//����
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

