#include "UIUtils.hpp"

NS_EE_BEGIN

void UIUtils::showContextMenu(Vector<Button*> &buttons)
{
	int counter = 0;
	for (auto button : buttons)
	{
		button->runAction(Sequence::create(DelayTime::create(0.02f * counter), Show::create(), NULL));
		counter++;
	}
}

void UIUtils::hideContextMenu(Vector<Button*> &buttons)
{
	for (auto button : buttons)
	{
		button->setVisible(false);
	}
}

NS_EE_END