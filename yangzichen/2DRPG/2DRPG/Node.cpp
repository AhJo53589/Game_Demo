#include "Node.h"


Node::Node() : mT(0.0f, 0.0f, 0.0f), mS(1.0f, 1.0f, 1.0f), mPatent(NULL)
{
	D3DXMatrixIdentity(&this->mRotationMatrix);
}

void Node::at(float x, float y, float z)
{
	this->mT[0] = x;
	this->mT[1] = y;
	this->mT[2] = z;

	this->notifyTranslationEvent(this->mT);
}

void Node::move(float x, float y, float z)
{
	this->mT[0] += x;
	this->mT[1] += y;
	this->mT[2] += z;

	this->notifyTranslationEvent(this->mT);
}

void Node::rotation(D3DXVECTOR3 axis, float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &axis, angle);

	this->mRotationMatrix = T;
}

void Node::rotation(D3DXMATRIX matrix)
{
	this->mRotationMatrix = matrix;
}

void Node::turn(D3DXVECTOR3 axis, float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &axis, angle);

	this->mRotationMatrix *= T;
}

void Node::turn(D3DXMATRIX matrix)
{
	this->mRotationMatrix *= matrix;
}

void Node::scaling(float x, float y, float z)
{
	this->mS[0] *= x;
	this->mS[1] *= y;
	this->mS[2] *= z;
}

void Node::size(float x, float y, float z)
{
	this->mS[0] = x;
	this->mS[1] = y;
	this->mS[2] = z;
}

D3DXMATRIX Node::getRotationMatrix()
{
	return this->mRotationMatrix;
}

D3DXMATRIX Node::getMoveMatrix()
{

	D3DXMATRIX t;
	D3DXMatrixTranslation(&t, this->mT[0], this->mT[1], this->mT[2]);

	return t;
}

D3DXMATRIX Node::getSizeMatrix()
{

	D3DXMATRIX t;
	D3DXMatrixScaling(&t, this->mS[0], this->mS[1], this->mS[2]);

	return t;
}

D3DXMATRIX Node::getMatrix()
{
	D3DXMATRIX matrix = this->getSizeMatrix();

	matrix *= this->getRotationMatrix();

	matrix *= this->getMoveMatrix();

	if (this->mPatent)
	{
		return   matrix * this->mPatent->getMatrix();
	}

	return  matrix;
}

void Node::addListener(MovementEventListener *l)
{
	if (l)
	{
		this->mListenerList.push_back(l);
	}
}

void Node::removeListener(MovementEventListener *l)
{
	for(std::vector<MovementEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		if(*it == l)
		{
			this->mListenerList.erase(it);
			return;
		}
	}
}

void Node::notifyTranslationEvent(D3DXVECTOR3 t)
{
	for(std::vector<MovementEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		(*it)->TranslationEvent(this, t);
	}
}