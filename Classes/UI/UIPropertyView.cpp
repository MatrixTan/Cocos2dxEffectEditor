//
//  UIPropertyView.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/31.
//
//

#include "UIPropertyView.hpp"
#include "UIControlPropertySlider.hpp"

NS_EE_BEGIN

UIPropertyView::UIPropertyView(Node* root)
:mOpened(false)
{
    mRootNode = root;
    
    Layout *rootLayout = static_cast<Layout*>(mRootNode);
    mOpenButton = static_cast<Button*>(ui::Helper::seekWidgetByName(rootLayout, "bt_property_open"));
    mOpenButton->addTouchEventListener(CC_CALLBACK_2(UIPropertyView::onOpenButtonTouchEvent, this));
    
    PropertySliderData *data = new PropertySliderData();
    data->messageName = "msg_hue_hue";
    data->defaultValue = 0.0f;
    data->maxValue = 1.0f;
    data->minValue = 0.0f;
    data->name = "色相";
    auto silder = new UIControlPropertySlider(root->getChildByName("color1"), data);
    PropertySliderData *data1 = new PropertySliderData();
    data1->messageName = "msg_hue_saturation";
    data1->defaultValue = 0.0f;
    data1->maxValue = 2.0f;
    data1->minValue = 0.0f;
    data1->name = "饱和度";
    auto silder1 = new UIControlPropertySlider(root->getChildByName("color2"), data1);
    PropertySliderData *data2 = new PropertySliderData();
    data2->messageName = "msg_hue_value";
    data2->defaultValue = 0.0f;
    data2->maxValue = 2.0f;
    data2->minValue = 0.0f;
    data2->name = "明度";
    auto silder2 = new UIControlPropertySlider(root->getChildByName("color3"), data2);
}

void UIPropertyView::onOpenButtonTouchEvent(cocos2d::Ref *sender, ui::Widget::TouchEventType type)
{
    if(type == ui::Widget::TouchEventType::ENDED)
    {
        auto onArrive = [&](){
            mOpenButton->setEnabled(true);
        };
        
        if(mOpened){
            mRootNode->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(300.0f, 0.0f)), CallFunc::create(onArrive),NULL));
            mOpened = false;
            mOpenButton->setEnabled(false);
            mOpenButton->setTitleText("<<");
        }else{
            mRootNode->runAction(Sequence::create(MoveBy::create(0.5f, Vec2(-300.0f, 0.0f)), CallFunc::create(onArrive),NULL));
            mOpened = true;
            mOpenButton->setEnabled(false);
            mOpenButton->setTitleText(">>");
        }
    }
}

NS_EE_END