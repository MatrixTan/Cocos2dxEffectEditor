//
//  UIControlPropertySlider.hpp
//  EffectEditor
//
//  Created by Matrix on 16/9/2.
//
//

#ifndef UIControlPropertySlider_hpp
#define UIControlPropertySlider_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MessageParam.hpp"

NS_EE_BEGIN

using namespace cocos2d;
using namespace ui;

class PropertySliderData
{
public:
    std::string name;
    std::string messageName;
    float minValue;
    float maxValue;
    float defaultValue;
};

class SliderMessageParam : public MessageParam
{
public:
    float value;
};

class UIControlPropertySlider
{
public:
    UIControlPropertySlider(Node* root, PropertySliderData* pdata);
    ~UIControlPropertySlider();
    
    void onSliderChange(Ref* sender, Slider::EventType event);
    
private:
    PropertySliderData *mData;
    Node* mNode;
    Slider* mSlider;
    TextField* mValueText;
};


NS_EE_END


#endif /* UIControlPropertySlider_hpp */
