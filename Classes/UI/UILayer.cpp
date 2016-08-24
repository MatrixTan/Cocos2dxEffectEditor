//
//  UILayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "UILayer.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "MainLayer.hpp"
#include "ShaderSprite.hpp"
#include "LinkEffect.hpp"
#include "RingEffect.hpp"
#include "ThunderLinkEffect.hpp"
#include <chrono>
#include "DrawLineLayer.hpp"

NS_EE_BEGIN

USING_NS_CC;

using namespace cocos2d::ui;

bool UILayer::init(void)
{
    mContainer = CSLoader::createNode("res/main_scene.csb");
    auto size = Director::getInstance()->getWinSize();
    this->addChild(mContainer, (int)SPRITE_ZORDER::UI);
    
    Layout *uiRoot = static_cast<Layout*>(mContainer->getChildByName("root_node"));
    Button *pButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_sprite"));
    pButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onUserTouchEvent, this));
    
    auto penButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_pen"));
    penButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onPenTouchEvent, this));
    
    mProjectView = new UIProjectView(mContainer->getChildByName("project_view"));
    
    auto lineLayer = DrawLineLayer::create();
    addChild(lineLayer);
    
    mState = UI_STATE::NONE;
    
    return Layer::init();
}

UI_STATE UILayer::getState()
{
    return mState;
}

void UILayer::onPenTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        mState = mState == UI_STATE::PEN ? UI_STATE::NONE : UI_STATE::PEN;
        auto pButton = static_cast<Button*>(sender);
        if(mState == UI_STATE::PEN){
            pButton->setColor(Color3B(200, 200, 255));
        }else{
            pButton->setColor(Color3B(255, 255, 255));
        }
    }
}

void UILayer::onUserTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
        auto link = ThunderLinkEffect::create(MainLayer::getInstance()->getSprite("7")
                                              , MainLayer::getInstance()->getSprite("6")
                                              , 1.5);
        addChild(link);
        
        /*std::string csbFile = "res/horizontal_effect2.csb";
        auto horizontalEffect = cocos2d::CSLoader::createNode(csbFile);
        auto timeLine = CSLoader::createTimeline(csbFile);
        addChild(horizontalEffect, 9999999);
        horizontalEffect->setPosition(Vec2(300, 300));
        horizontalEffect->runAction(timeLine);
        timeLine->setLastFrameCallFunc([horizontalEffect](){
            horizontalEffect->removeFromParent();
        });
        timeLine->gotoFrameAndPlay(0, false);
         */
        // ===================
        
        /*auto shaderSprite = MainLayer::getInstance()->getSprite("1");
        auto ringEffect = RingEffect::create(300.0f, 2.0f);
        addChild(ringEffect);
        ringEffect->setPosition(shaderSprite->getPosition());
         */
        
        /*shaderSprite->runAction(Sequence::create(DelayTime::create(2.0f),
                                                                       MoveBy::create(2.0f, Vec2(400.0f, 0.0f)),
                                                                       DelayTime::create(1.0f),
                                                                       MoveBy::create(2.0f, Vec2(-400.0f, 0.0f)),
                                                                       RemoveSelf::create(true), NULL));
          */
        //shaderSprite->resetTimeUniform();
        
        //auto sprite2 = MainLayer::getInstance()->getSprite("2");
        //auto link = LinkEffect::create(sprite2, shaderSprite, 500.0f);
        //addChild(link);
        
        /*auto test = ParticleSystemQuad::create("res/link_light.plist");
        addChild(test, 99999);
        test->setAnchorPoint(Vec2(0.0f, 0.0f));
        test->setPosition(shaderSprite->getPosition());
        test->setRotation(60);*/
    }
}

NS_EE_END