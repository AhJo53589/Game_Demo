#include "UI.h"
#include "Define2DRPG.h"
#include "Res.h"

UIPanel::UIPanel(float x, float y, float w, float h) : mShow(true)
{
	this->setPosition(x, y);
	this->setSize(w, h);
}

UIPanel::~UIPanel()
{
	for(std::vector<UIPanel *>::iterator it=this->mChildrenList.begin(); it!=this->mChildrenList.end(); it++)
	{
		Delete((*it));
	}
}

void UIPanel::setPosition(float x, float y)
{
	this->mX = x;
	this->mY = y;
	this->at(x, -y, this->mT[2]);
}

void UIPanel::setSize(float w, float h)
{
	if (w == 0)
		w = TILESIDE;
	if (h == 0)
		h = TILESIDE;

	this->mW = w;
	this->mH = h;
	D3DXMatrixScaling(&this->mUISize, w/TILESIDE, h/TILESIDE, 1.0f);
}

D3DXMATRIX UIPanel::getUIDrawMatrix()
{
	D3DXMATRIX t;
	D3DXMatrixTranslation(&t, TILESIDE / 2.0f, -TILESIDE / 2.0f, this->mT[2]);
	return t * this->mUISize * Node::getMatrix();
}

D3DXVECTOR2 UIPanel::getLocalPointFromScreen(float x, float y)
{
	if (this->mPatent)
	{
		y *= -1;
		D3DXVECTOR3 in(x, y, 0.0f), out;
		D3DXMATRIX m;
		D3DXMatrixInverse(&m, NULL, &this->mPatent->getMatrix());
		D3DXVec3TransformCoord(&out, &in, &m);
		return D3DXVECTOR2(out[0], -out[1]);
	}
	else
	{
		return D3DXVECTOR2(x, y);
	}
}

D3DXVECTOR2 UIPanel::getScreenPointFromLocal(float x, float y)
{
	if (this->mPatent)
	{
		y *= -1;
		D3DXVECTOR3 in(x, y, 0.0f), out;
		D3DXMATRIX m;
		D3DXVec3TransformCoord(&out, &in, &this->mPatent->getMatrix());
		return D3DXVECTOR2(out[0], -out[1]);
	}
	else
	{
		return D3DXVECTOR2(x, y);
	}
}

void UIPanel::InputEvent(DInput *n, float dt)
{
	for(std::vector<UIPanel *>::iterator it=this->mChildrenList.begin(); it!=this->mChildrenList.end(); it++)
	{
		(*it)->InputEvent(n, dt);
	}
}

void UIPanel::update(float dt)
{
	if(!mShow)
		return;

	for(std::vector<UIPanel *>::iterator it=this->mChildrenList.begin(); it!=this->mChildrenList.end(); it++)
	{
		(*it)->update(dt);
	}
}

void UIPanel::draw() 
{
	if(!mShow)
		return;

	for(std::vector<UIPanel *>::iterator it=this->mChildrenList.begin(); it!=this->mChildrenList.end(); it++)
	{
		(*it)->draw();
	}
}

void UIPanel::addChild(UIPanel* child)
{
	this->mChildrenList.push_back(child);
	child->mPatent = this;
}

UIButton::UIButton(float x, float y, float w, float h, TCHAR *s) : UIPanel(x, y, w, h), mText(s), mButtonState(BUTTON_NORMAL)
{
	this->mImage[0] = BUTTON1;
	this->mImage[1] = BUTTON2;
	this->mImage[2] = BUTTON3;
}

void UIButton::update(float dt)
{
	if(!mShow)
		return;

	UIPanel::update(dt);
}

void UIButton::draw()
{	
	if(!mShow)
		return;

	game->getRes()->getModel()->draw(this->mImage[this->mButtonState], this->getUIDrawMatrix(), game->getUICamera());

	D3DXVECTOR2 v = this->getScreenPointFromLocal(this->mX, this->mY);
	RECT R = {(LONG)v[0]+45, (LONG)v[1]+10, 0, 0};
	game->getRes()->getFont()->DrawText(0, this->mText, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(255, 255, 255));

	UIPanel::draw();
}

void UIButton::InputEvent(DInput *n, float dt)
{
	if(!mShow)
		return;

	//获取指针位置 并从 窗口坐标系转换为dx坐标系
	POINT point;
	GetCursorPos(&point);
	ScreenToClient(game->getWnd(),&point);
	D3DXVECTOR2 v = this->getLocalPointFromScreen((float)point.x, (float)point.y);
	
	if (v[0] < this->mX || v[1] < this->mY ||
		v[0] > this->mX + this->mW || v[1] > this->mY + this->mH)
	{
		this->mButtonState = BUTTON_NORMAL;
	}
	else
	{
		if (n->mouseButtonDown(0))
		{
			this->mButtonState = BUTTON_DOWN;
			this->notifyClickEvent();
		}
		else
		{
			this->mButtonState = BUTTON_HOVER;
		}
	}

	UIPanel::InputEvent(n, dt);
}

void UIButton::addListener(ButtonEventListener *l)
{
	if (l)
	{
		this->mListenerList.push_back(l);
	}
}

void UIButton::removeListener(ButtonEventListener *l)
{
	for(std::vector<ButtonEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		if(*it == l)
		{
			this->mListenerList.erase(it);
			return;
		}
	}
}

void UIButton::notifyClickEvent()
{
	for(std::vector<ButtonEventListener *>::iterator it=this->mListenerList.begin(); it!=this->mListenerList.end(); it++) 
	{
		(*it)->ClickEvent(this);
	}
}

UIForm::UIForm(float x, float y, float w, float h, TCHAR *s) : UIPanel(x, y, w, h), mText(s), mBackground(FORM)
{
}

void UIForm::draw()
{
	if(!mShow)
		return;

	game->getRes()->getModel()->draw(this->mBackground, this->getUIDrawMatrix(), game->getUICamera());

	D3DXVECTOR2 v = this->getScreenPointFromLocal(this->mX, this->mY);
	RECT R = {(LONG)v[0]+65, (LONG)v[1]+10, 0, 0};
	game->getRes()->getFont()->DrawText(0, this->mText, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0));

	UIPanel::draw();
}