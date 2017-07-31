//
//  UIControlPropertyCommon.cpp
//  EffectEditor
//
//  Created by Matrix on 2017/3/1.
//
//

#include "UIControlPropertyCommon.hpp"

using namespace cocos2d::ui;

NS_EE_BEGIN

UIControlPropertyCommon::UIControlPropertyCommon(){

    auto node = CSLoader::createNode("res/control_property_common.csb");
    addChild(node);
    
    mNode = static_cast<ui::Widget*>(node);
    
}

void UIControlPropertyCommon::setCurrentSprite(ee::ShaderSprite *sprite){
    
}

NS_EE_END
