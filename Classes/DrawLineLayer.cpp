//
//  DrawLineLayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/11.
//
//

#include "DrawLineLayer.hpp"

NS_EE_BEGIN

DrawLineLayer* DrawLineLayer::create()
{
    auto pLayer = new(std::nothrow)DrawLineLayer();
    if(pLayer && pLayer->init()){
        pLayer->autorelease();
        return pLayer;
    }
    CC_SAFE_DELETE(pLayer);
    return nullptr;
}

bool DrawLineLayer::init()
{
    auto pNode = DrawNode::create();
    addChild(pNode);
    pNode->drawLine(Vec2(100, 100), Vec2(300, 300), Color4F(1.0, 1.0, 0.0, 1.0));
    pNode->drawQuadBezier(Vec2(100, 100), Vec2(100, 300), Vec2(300, 300), 100, Color4F(1.0, 0.0, 0.0, 1.0));
    pNode->drawCubicBezier(Vec2(100, 100), Vec2(300, 100), Vec2(300, 100), Vec2(300, 100), 100, Color4F(1.0, 0.0, 0.0, 1.0));
    return Layer::init();
};

bool DrawLineLayer::onTouchBegin(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
    return true;
}

void DrawLineLayer::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void DrawLineLayer::onTouchEnd(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto pos = touch->getLocation();
    
}

void DrawLineLayer::onTouchCancel(Touch *touch, Event *event)
{
    
}


NS_EE_END