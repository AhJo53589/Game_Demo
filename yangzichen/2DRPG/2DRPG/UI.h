#pragma once
#include "node.h"
#include "DInput.h"

class UIPanel : public Node, public InputEventListener
{
public:
	UIPanel(float x, float y, float w = 0, float h = 0);
	virtual ~UIPanel();

	virtual void update(float dt);
	virtual void draw();

	virtual void addChild(UIPanel *c);

	virtual void InputEvent(DInput *n, float dt);

	virtual void setPosition(float x, float y);
	virtual void setSize(float w, float h);
	virtual D3DXVECTOR2 getSize() {return D3DXVECTOR2(this->mW, this->mH);}
	virtual void setZder(float z) {this->mT[2] = z;}

	virtual D3DXMATRIX getUIDrawMatrix();
	virtual D3DXVECTOR2 getLocalPointFromScreen(float x, float y);
	virtual D3DXVECTOR2 getScreenPointFromLocal(float x, float y);

	bool mShow;

protected:
	float mX, mY, mW, mH;
	D3DXMATRIX mUISize;
	std::vector<UIPanel *> mChildrenList;
};

//Button 枚举
enum
{
	BUTTON_NORMAL = 0,
	BUTTON_HOVER,
	BUTTON_DOWN
};

class UIButton;

class ButtonEventListener
{
public:
	virtual void ClickEvent(UIButton *n) = 0;
};

class UIButton : public UIPanel
{
public:
	UIButton(float x, float y, float w, float h, TCHAR *s);
	virtual ~UIButton() {}

	virtual void update(float dt);
	virtual void draw();

	virtual void InputEvent(DInput *n, float dt);

	void addListener(ButtonEventListener *l);
    void removeListener(ButtonEventListener *l);

protected:
	int mImage[3];
	int mButtonState;
	TCHAR *mText;

	std::vector<ButtonEventListener *> mListenerList;

	void notifyClickEvent();
};

class UIForm : public UIPanel, public ButtonEventListener
{
public:
	UIForm(float x, float y, float w, float h, TCHAR *s);
	virtual ~UIForm() {}

	virtual void draw();
	virtual void ClickEvent(UIButton *n){}

protected:
	int mBackground;
	TCHAR *mText;
};