#ifndef UI_CONTEXT_MENU_LAYER_HPP
#define UI_CONTEXT_MENU_LAYER_HPP

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "ui/CocosGUI.h"
#include "Common/NodeSingleton.hpp"

NS_EE_BEGIN
USING_NS_CC;
using namespace ui;

enum UIMenuButtonType
{
	None = -1,
	File = 0,
	Edit = 1,
	Tool = 2
};


class UIContextMenuLayer : public NodeSingleton <UIContextMenuLayer>, Layer
{
public:
	CREATE_FUNC(UIContextMenuLayer);
	virtual bool init() override;

	void setMenuButtons(UIMenuButtonType type, Vector<Button*>& buttons);
	void showContextMenu(UIMenuButtonType type);

private:
	bool onLayerTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
	void clearCurrentMenu();

private:
	std::map<int, Vector<Button*> > mMenuButtons;
	UIMenuButtonType mCurrentButtonType = UIMenuButtonType::None;

};

NS_EE_END

#endif // !UI_CONTEXT_MENU_LAYER_HPP
