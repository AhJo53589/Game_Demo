#pragma once
#include "UI.h"

class RightMenu : public UIForm
{
public:
	RightMenu();
	virtual ~RightMenu() {}
	virtual void ClickEvent(UIButton *n);

protected:
	UIButton *mInfoButton, *mSkillButton, *mItemButton, *mCloseButton;
};

class InfoForm : public UIForm
{
public:
	InfoForm();
	virtual ~InfoForm() {}
	virtual void ClickEvent(UIButton *n);

protected:
	UIButton *mCloseButton;
};

class SkillForm : public UIForm
{
public:
	SkillForm();
	virtual ~SkillForm() {}
	virtual void ClickEvent(UIButton *n);

protected:
	UIButton *mCloseButton;
};

class ItemForm : public UIForm
{
public:
	ItemForm();
	virtual ~ItemForm() {}
	virtual void ClickEvent(UIButton *n);

protected:
	UIButton *mCloseButton;
};

class UIManager : public UIPanel
{
public:
	UIManager();
	~UIManager() {}

	virtual void InputEvent(DInput *n, float dt);

	UIForm *getInfoForm() {return this->mInfoForm;}
	UIForm *getSkillForm() {return this->mSkillForm;}
	UIForm *getItemForm() {return this->mItemForm;}
protected:

	UIForm *mRightMenu, *mInfoForm, *mSkillForm, *mItemForm;

};

