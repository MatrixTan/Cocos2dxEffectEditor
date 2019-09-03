
#include "UI/UIParticleEditor.hpp"
#include "ui/UIButton.h"

NS_EE_BEGIN



bool UIParticleEditor::init()
{

	ui::Button *pButton = ui::Button::create("Default/Button_Normal.png", "Default/Button_Press.png", "Default/Button_Disable.png");
	addChild(pButton);
	pButton->setPosition(Vec2(100.0f, -100.0f));
	pButton->addTouchEventListener(CC_CALLBACK_2(UIParticleEditor::onButtonTouchEvent, this));
	return Layer::init();
}


void UIParticleEditor::onButtonTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
	if (type == Widget::TouchEventType::ENDED)
	{
		///Do something.
	}
}

NS_EE_END