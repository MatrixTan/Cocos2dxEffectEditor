#include "UI/UIContextMenuLayer.hpp"
#include "UI/UIUtils.hpp"

NS_EE_BEGIN

bool UIContextMenuLayer::init()
{
	auto dispacher = Director::getInstance()->getEventDispatcher();
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(UIContextMenuLayer::onLayerTouchBegan, this);
	listener->setSwallowTouches(false);
	dispacher->addEventListenerWithSceneGraphPriority(listener, this);

	return Layer::init();
}

bool UIContextMenuLayer::onLayerTouchBegan(CCTouch* pTouch, CCEvent *pEvent)
{
	clearCurrentMenu();
	return true;
}

void UIContextMenuLayer::clearCurrentMenu()
{
	if (mCurrentButtonType != UIMenuButtonType::None)
	{
		UIUtils::hideContextMenu(mMenuButtons.at(mCurrentButtonType));
		mCurrentButtonType = UIMenuButtonType::None;
	}
}

void UIContextMenuLayer::showContextMenu(UIMenuButtonType type)
{
	mCurrentButtonType = type;
	UIUtils::showContextMenu(mMenuButtons.at(mCurrentButtonType));
}

NS_EE_END