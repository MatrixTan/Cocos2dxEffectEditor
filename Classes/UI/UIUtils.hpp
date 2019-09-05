#ifndef UI_UTILS_HPP
#define UI_UTILS_HPP

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "ui/CocosGUI.h"

NS_EE_BEGIN
USING_NS_CC;
using namespace ui;

class UIUtils
{
public:
	static void showContextMenu(Vector<Button*>& buttons);
	static void hideContextMenu(Vector<Button*>& buttons);

};

NS_EE_END

#endif // !UI_UTILS_HPP

