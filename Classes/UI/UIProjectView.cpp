//
//  UIProjectView.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/24.
//
//

#include "UIProjectView.hpp"


NS_EE_BEGIN

UIProjectView::UIProjectView(Node* root)
:mOpened(false)
{
    mRootNode = root;
    
    Layout *rootLayout = static_cast<Layout*>(mRootNode);
    mOpenButton = static_cast<Button*>(ui::Helper::seekWidgetByName(rootLayout, "bt_project_open"));
    mOpenButton->addTouchEventListener(CC_CALLBACK_2(UIProjectView::onOpenButtonTouchEvent, this));
}

void UIProjectView::onOpenButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED)
    {
        auto onArrive = [&](){
            mOpenButton->setEnabled(true);
        };
        
        if(mOpened){
            mRootNode->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-200.0f, 0.0f)), CallFunc::create(onArrive),NULL));
            mOpened = false;
            mOpenButton->setEnabled(false);
            mOpenButton->setTitleText(">>");
        }else{
            mRootNode->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(200.0f, 0.0f)), CallFunc::create(onArrive),NULL));
            mOpened = true;
            mOpenButton->setEnabled(false);
            mOpenButton->setTitleText("<<");
        }
    }
}

NS_EE_END