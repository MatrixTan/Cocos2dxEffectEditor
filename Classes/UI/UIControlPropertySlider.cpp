//
//  UIControlPropertySlider.cpp
//  EffectEditor
//
//  Created by Matrix on 16/9/2.
//
//

#include "UIControlPropertySlider.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MessageDispatcher.hpp"

NS_EE_BEGIN

using namespace cocos2d::ui;

UIControlPropertySlider::UIControlPropertySlider(Node* root, PropertySliderData* data)
{
    mNode = root;
    mData = data;
    
    Layout *rootLayout = static_cast<Layout*>(mNode);
    auto labelName = static_cast<Text*>(ui::Helper::seekWidgetByName(rootLayout, "lb_name"));
    labelName->setString(mData->name);
    auto labelMin = static_cast<Text*>(ui::Helper::seekWidgetByName(rootLayout, "lb_min"));
    labelMin->setString(std::to_string(mData->minValue));
    auto labelMax = static_cast<Text*>(ui::Helper::seekWidgetByName(rootLayout, "lb_max"));
    labelMax->setString(std::to_string(mData->maxValue));
    auto textValue = static_cast<TextField*>(ui::Helper::seekWidgetByName(rootLayout, "tb_value"));
    textValue->setString(std::to_string(mData->defaultValue));
    mSlider = static_cast<Slider*>(ui::Helper::seekWidgetByName(rootLayout, "slider"));
    mSlider->addEventListener(CC_CALLBACK_2(UIControlPropertySlider::onSliderChange, this));

}

UIControlPropertySlider::~UIControlPropertySlider()
{
    CC_SAFE_DELETE(mData);
}

void UIControlPropertySlider::onSliderChange(cocos2d::Ref *sender, Slider::EventType event)
{
    if(event == Slider::EventType::ON_PERCENTAGE_CHANGED)
    {
        SliderMessageParam param;
        param.value = mData->minValue + (mData->maxValue - mData->minValue) * mSlider->getPercent() / 100.0f;
        MessageDispatcher::getInstance()->notifyMessage(mData->messageName, this, &param);
    }
    
}

NS_EE_END