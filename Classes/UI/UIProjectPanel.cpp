#include "UI/UIProjectPanel.hpp"
#include "UI/UIUtils.hpp"


NS_EE_BEGIN

bool UIProjectPanel::init(void)
{
	Scale9Sprite *pBg = Scale9Sprite::create("ui/panel_bg.png");
	addChild(pBg);
	pBg->setContentSize(Size(280, 950));
	pBg->setColor(Color3B(37, 37, 37));
	pBg->setAnchorPoint(Vec2(0.0f, 0.0f));
	pBg->setPosition(Vec2(0.0f ,0.0f));

	Scale9Sprite *pBottomBg = Scale9Sprite::create("ui/panel_bg.png");
	addChild(pBottomBg);
	pBottomBg->setContentSize(Size(280, 44));
	pBottomBg->setColor(Color3B(16, 16, 16));
	pBottomBg->setAnchorPoint(Vec2(0.0f, 0.0f));
	pBottomBg->setPosition(Vec2(0.0f, 0.0f));

	Button* addButton = Button::create("ui/button_add.png", "ui/button_add.png", "ui/button_add.png");
	addButton->addClickEventListener(CC_CALLBACK_1(UIProjectPanel::onAdd, this));
	addButton->setPosition(Vec2(24.0f, 24.0f));
	pBottomBg->addChild(addButton);

	mAddButtons.pushBack(createButton("Particle", Vec2(5.0f, 45.0f), CC_CALLBACK_1(UIProjectPanel::onAddParticle, this)));
	UIUtils::hideContextMenu(mAddButtons);

	return Layer::init();
}

void UIProjectPanel::onAdd(Ref* sender)
{
	UIUtils::showContextMenu(mAddButtons);
}

void UIProjectPanel::onAddParticle(Ref* sender)
{

}

Button* UIProjectPanel::createButton(const std::string& title, const Vec2& pos, const Widget::ccWidgetClickCallback &callback)
{
	Button *pButton = Button::create("ui/menu_button_normal.png", "ui/menu_button_press.png", "ui/menu_button_disable.png");
	pButton->setTitleText(title);
	pButton->setTitleColor(Color3B::BLACK);
	pButton->setTitleFontSize(16.0f);
	pButton->setScale9Enabled(true);
	pButton->setSize(Size(70, 30));
	pButton->setAnchorPoint(Vec2(0.0f, 0.0f));
	addChild(pButton);
	pButton->setPosition(pos);
	pButton->addClickEventListener(callback);
	return pButton;
}


NS_EE_END