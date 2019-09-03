#ifndef UI_MENU_BAR_HPP
#define UI_MENU_BAR_HPP

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "ui/CocosGUI.h"

NS_EE_BEGIN
USING_NS_CC;

class UIMenuBar : public Layer
{
public:
	virtual bool init() override;

	CREATE_FUNC(UIMenuBar);
};


NS_EE_END


#endif // !UI_MENU_BAR_HPP
