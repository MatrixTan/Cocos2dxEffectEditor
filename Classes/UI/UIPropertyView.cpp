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
    mRootNode->addChild(silder);
    silder->setPosition(Vec2(125.0f, 300.0f));
    
    PropertySliderData *data1 = new PropertySliderData();
    data1->messageName = "msg_hue_saturation";
    data1->defaultValue = 0.0f;
    data1->maxValue = 2.0f;
    data1->minValue = 0.0f;
    data1->name = "Saturability";
    auto silder1 = new UIControlPropertySlider(root->getChildByName("color2"), data1);
    mRootNode->addChild(silder1);
    silder1->setPosition(Vec2(125.0f, 250.0f));
    
    PropertySliderData *data2 = new PropertySliderData();
    data2->messageName = "msg_hue_value";
    data2->defaultValue = 0.0f;
    data2->maxValue = 2.0f;
    data2->minValue = 0.0f;
    data2->name = "明度";
    auto silder2 = new UIControlPropertySlider(root->getChildByName("color3"), data2);
    mRootNode->addChild(silder2);
    silder2->setPosition(Vec2(125.0f, 200.0f));
    
    mCommonProperty = new UIControlPropertyCommon();
    mRootNode->addChild(mCommonProperty);
    mCommonProperty->setPosition(Vec2(125.0f, 400.0f));
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

void UIPropertyView::setCurrentSprite(ee::ShaderSprite *sprite){
    mCommonProperty->setCurrentSprite(sprite);
}

NS_EE_END
