#include "UI/UIMenuBar.hpp"
#include "MainScene.hpp"
#include "UI/UILayer.hpp"
#include "Common/PlatformAdapter.h"
#include "UI/UIUtils.hpp"
#include "UI/UIContextMenuLayer.hpp"

NS_EE_BEGIN


bool UIMenuBar::init()
{
	mButtons.pushBack(createButton("File", Vec2(50 + 72 * UIMenuButtonType::File, 0), CC_CALLBACK_1(UIMenuBar::onFileButtonClick, this)));
	Vector<Button*> fileButtons;
	fileButtons.pushBack(createButton("New", Vec2(50 + 72 * UIMenuButtonType::File, -31 * 1), CC_CALLBACK_1(UIMenuBar::onNew, this)));
	fileButtons.pushBack(createButton("Open", Vec2(50 + 72 * UIMenuButtonType::File, -31 * 2), CC_CALLBACK_1(UIMenuBar::onOpen, this)));
	fileButtons.pushBack(createButton("Save", Vec2(50 + 72 * UIMenuButtonType::File, -31 * 3), CC_CALLBACK_1(UIMenuBar::onSave, this)));
	fileButtons.pushBack(createButton("Export", Vec2(50 + 72 * UIMenuButtonType::File, -31 * 4), CC_CALLBACK_1(UIMenuBar::onExport, this)));
	UIUtils::hideContextMenu(fileButtons);
	mMenuButtons.push_back(fileButtons);

	mButtons.pushBack(createButton("Edit", Vec2(50 + 72 * UIMenuButtonType::Edit, 0), CC_CALLBACK_1(UIMenuBar::onEditButtonClick, this)));
	Vector<Button*> editButtons;
	editButtons.pushBack(createButton("Delete", Vec2(50 + 72 * UIMenuButtonType::Edit, -31 * 1), CC_CALLBACK_1(UIMenuBar::onDelete, this)));
	UIUtils::hideContextMenu(editButtons);
	mMenuButtons.push_back(editButtons);

	mButtons.pushBack(createButton("Tool", Vec2(50 + 72 * UIMenuButtonType::Tool, 0), CC_CALLBACK_1(UIMenuBar::onToolButtonClick, this)));
	Vector<Button*> toolButtons;
	toolButtons.pushBack(createButton("Pen", Vec2(50 + 72 * UIMenuButtonType::Tool, -31 * 1), CC_CALLBACK_1(UIMenuBar::onPen, this)));
	UIUtils::hideContextMenu(toolButtons);
	mMenuButtons.push_back(toolButtons);

	

	return Layer::init();
}


void UIMenuBar::onFileButtonClick(Ref* sender)
{
	UIContextMenuLayer::getInstance()->showContextMenu(UIMenuButtonType::File);
}

void UIMenuBar::onEditButtonClick(Ref* sender)
{
	UIContextMenuLayer::getInstance()->showContextMenu(UIMenuButtonType::Edit);
}

void UIMenuBar::onToolButtonClick(Ref* sender)
{
	UIContextMenuLayer::getInstance()->showContextMenu(UIMenuButtonType::Tool);
}


Button* UIMenuBar::createButton(const std::string& title, const Vec2& pos, const Widget::ccWidgetClickCallback &callback)
{
	Button *pButton = Button::create("ui/menu_button_normal.png", "ui/menu_button_press.png", "ui/menu_button_disable.png");
	pButton->setTitleText(title);
	pButton->setTitleColor(Color3B::BLACK);
	pButton->setTitleFontSize(16.0f);
	pButton->setScale9Enabled(true);
	pButton->setSize(Size(70, 30));
	addChild(pButton);
	pButton->setPosition(pos);
	pButton->addClickEventListener(callback);
	return pButton;
}


void UIMenuBar::onNew(Ref* sender)
{
	MainScene::getInstance()->newProject();
}

void UIMenuBar::onOpen(Ref* sender)
{
	std::string path = PlatformAdapter::getFilePath("json");
	if (path.length() > 0) {
		MainScene::getInstance()->loadProject(path);
	}
}

void UIMenuBar::onSave(Ref* sender)
{
	std::string savePath = PlatformAdapter::getSaveFilePath("json");
	if (MainScene::getInstance()->getCurrentProject()->saveProject(savePath)) {
		 UILayer::getInstance()->setStatus("Save Complete!");
	}
	else 
	{
		UILayer::getInstance()->setStatus("Save Failed!");
	}
}

void UIMenuBar::onExport(Ref* sender)
{

}

void UIMenuBar::onDelete(Ref* sender)
{

}

void UIMenuBar::onPen(Ref* sender)
{

}


NS_EE_END