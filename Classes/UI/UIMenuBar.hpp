#ifndef UI_MENU_BAR_HPP
#define UI_MENU_BAR_HPP

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "ui/CocosGUI.h"
#include "UI/UIContextMenuLayer.hpp"

NS_EE_BEGIN
USING_NS_CC;
using namespace ui;


class UIMenuBar : public Layer
{
public:
	virtual bool init() override;

	CREATE_FUNC(UIMenuBar);

private:
	Button* createButton(const std::string& title, const Vec2& pos, const Widget::ccWidgetClickCallback &callback);

	void onFileButtonClick(cocos2d::Ref *sender);
	void onEditButtonClick(cocos2d::Ref *sender);
	void onToolButtonClick(cocos2d::Ref *sender);

	void onNew(Ref *sender);
	void onOpen(Ref *sender);
	void onSave(Ref *sender);
	void onExport(Ref *sender);
	void onDelete(Ref *sender);
	void onPen(Ref *sender);

private:
	Vector<Button*> mButtons;
	std::vector< Vector<Button*> > mMenuButtons;
	UIMenuButtonType mCurrentButtonType = UIMenuButtonType::None;
};


NS_EE_END


#endif // !UI_MENU_BAR_HPP
