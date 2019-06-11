#include "UIManager.h"
#include "Define2DRPG.h"

RightMenu::RightMenu() : UIForm(0, 0, 160, 160, TEXT(""))
{
	this->mInfoButton = new UIButton(20, 14, 120, 30, TEXT("状态"));
	this->mSkillButton = new UIButton(20, 48, 120, 30, TEXT("技能"));
	this->mItemButton = new UIButton(20, 82, 120, 30, TEXT("道具"));
	this->mCloseButton = new UIButton(20, 116, 120, 30, TEXT("关闭"));

	this->mInfoButton->addListener(this);
	this->mSkillButton->addListener(this);
	this->mItemButton->addListener(this);
	this->mCloseButton->addListener(this);

	this->addChild(this->mInfoButton);
	this->addChild(this->mSkillButton);
	this->addChild(this->mItemButton);
	this->addChild(this->mCloseButton);
}

void RightMenu::ClickEvent(UIButton *n)
{
	if (n == this->mInfoButton)
	{
		game->getUIManager()->getInfoForm()->mShow = true;
	}
	else if (n == this->mSkillButton)
	{
		game->getUIManager()->getSkillForm()->mShow = true;
	}
	else if (n == this->mItemButton)
	{
		game->getUIManager()->getItemForm()->mShow = true;
	}
	else if (n == this->mCloseButton)
	{
		this->mShow = false;
	}
}

InfoForm::InfoForm() : UIForm(40, 40, 160, 160, TEXT("状态"))
{
	this->mCloseButton = new UIButton(20, 116, 120, 30, TEXT("关闭"));

	this->mCloseButton->addListener(this);

	this->addChild(this->mCloseButton);
}

void InfoForm::ClickEvent(UIButton *n)
{
	if (n == this->mCloseButton)
	{
		this->mShow = false;
	}
}

SkillForm::SkillForm() : UIForm(200, 40, 160, 160, TEXT("技能"))
{
	this->mCloseButton = new UIButton(20, 116, 120, 30, TEXT("关闭"));

	this->mCloseButton->addListener(this);

	this->addChild(this->mCloseButton);
}

void SkillForm::ClickEvent(UIButton *n)
{
	if (n == this->mCloseButton)
	{
		this->mShow = false;
	}
}

ItemForm::ItemForm() : UIForm(360, 40, 160, 160, TEXT("道具"))
{
	this->mCloseButton = new UIButton(20, 116, 120, 30, TEXT("关闭"));

	this->mCloseButton->addListener(this);

	this->addChild(this->mCloseButton);
}

void ItemForm::ClickEvent(UIButton *n)
{
	if (n == this->mCloseButton)
	{
		this->mShow = false;
	}
}

UIManager::UIManager() : UIPanel(0, 0)
{
	this->mRightMenu = new RightMenu();
	this->mRightMenu->mShow = false;
	this->mInfoForm = new InfoForm();
	this->mInfoForm->mShow = false;
	this->mSkillForm = new SkillForm();
	this->mSkillForm->mShow = false;
	this->mItemForm = new ItemForm();
	this->mItemForm->mShow = false;

	this->addChild(this->mInfoForm);
	this->addChild(this->mSkillForm);
	this->addChild(this->mItemForm);
	this->addChild(this->mRightMenu);
}

void UIManager::InputEvent(DInput *n, float dt)
{
	//如果鼠标右键被按下且在屏幕范围内 显示右键菜单
	if (n->mouseButtonDown(1))
	{
		//获取指针位置 并从 窗口坐标系转换为dx坐标系
		POINT point;
		GetCursorPos(&point);
		ScreenToClient(game->getWnd(),&point);
		D3DXVECTOR2 v = this->getLocalPointFromScreen((float)point.x, (float)point.y);

		if(v[0] > 0 && v[1] > 0 && v[0] < RESOLUTIONW && v[1] < RESOLUTIONH)
		{
			D3DXVECTOR2 size = this->mRightMenu->getSize();

			if (v[0] + size[0] > RESOLUTIONW)
				v[0] = RESOLUTIONW - size[0];
			if (v[1] + size[1] > RESOLUTIONH)
				v[1] = RESOLUTIONH - size[1];

			this->mRightMenu->setPosition(v[0], v[1]);
			this->mRightMenu->mShow = true;
		}
	}

	UIPanel::InputEvent(n, dt);
}