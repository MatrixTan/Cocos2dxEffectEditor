#ifndef UI_PROJECT_PANEL_HPP
#define UI_PROJECT_PANEL_HPP

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "ui/CocosGUI.h"

NS_EE_BEGIN
USING_NS_CC;
using namespace ui;

class UIProjectPanel : public Layer
{
public:
	CREATE_FUNC(UIProjectPanel);
	virtual bool init(void) override;

	

private:
	void onAdd(Ref* sender);
	void onAddParticle(Ref* sender);
	Button* createButton(const std::string& title, const Vec2& pos, const Widget::ccWidgetClickCallback &callback);

private:
	Vector<Button*> mAddButtons;
};

NS_EE_END
#endif // ! UI_PROJECT_PANEL_HPP
