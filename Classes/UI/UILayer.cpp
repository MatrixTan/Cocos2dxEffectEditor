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
#include "Project.hpp"
#include "MainScene.hpp"
#include "UIControlPropertySlider.hpp"
#include "MessageDispatcher.hpp"
#include "AudioPlayer.hpp"
#include "ActionEx.hpp"

NS_EE_BEGIN

USING_NS_CC;

using namespace cocos2d::ui;

bool UILayer::init(void)
{
    mContainer = CSLoader::createNode("res/main_scene.csb");
    auto size = Director::getInstance()->getWinSize();
    this->addChild(mContainer, (int)SPRITE_ZORDER::UI);
    
    bindListener();
    
    mProjectView = new UIProjectView(mContainer->getChildByName("project_view"));
    mPropertyView = new UIPropertyView(mContainer->getChildByName("property_view"));
    
    auto lineLayer = DrawLineLayer::create();
    addChild(lineLayer);
    
    mState = UI_STATE::NONE;
    
    return Layer::init();
}

void UILayer::bindListener()
{
    Layout *uiRoot = static_cast<Layout*>(mContainer->getChildByName("root_node"));
    mStatusText = static_cast<Text*>(mContainer->getChildByName("tb_status"));
    
    auto *pTest1Button = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_test1"));
    pTest1Button->addTouchEventListener(CC_CALLBACK_2(UILayer::onTest1Event, this));
    auto *pTest2Button = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_test2"));
    pTest2Button->addTouchEventListener(CC_CALLBACK_2(UILayer::onTest2Event, this));
    auto *pTest3Button = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_test3"));
    pTest3Button->addTouchEventListener(CC_CALLBACK_2(UILayer::onTest3Event, this));
    
    auto *pButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_sprite"));
    pButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onUserTouchEvent, this));
    
    auto penButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_pen"));
    penButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onPenTouchEvent, this));
    
    auto saveButton = static_cast<Button*>(ui::Helper::seekWidgetByName(uiRoot, "bt_save"));
    saveButton->addTouchEventListener(CC_CALLBACK_2(UILayer::onSaveTouchEvent, this));
    mDrawPanel =  new UIDrawView(mContainer->getChildByName("draw_panel"));
    mDrawPanel->setVisible(false);
    
    MessageDispatcher::getInstance()->addListener("msg_hue_hue", this, std::bind(&UILayer::onSliderMessage1, this, std::placeholders::_1, std::placeholders::_2));
    MessageDispatcher::getInstance()->addListener("msg_hue_saturation", this, std::bind(&UILayer::onSliderMessage2, this, std::placeholders::_1, std::placeholders::_2));
    MessageDispatcher::getInstance()->addListener("msg_hue_value", this, std::bind(&UILayer::onSliderMessage3, this, std::placeholders::_1, std::placeholders::_2));
    
    auto effectNode = Node::create();
    auto effectHead = Sprite::create("projects/project1/2.png");
    effectHead->setAnchorPoint(Vec2(0.5f, 0.0f));
    //effectHead->setPosition(Vec2(0.0f, 40.0f));
    auto effectStreak = MotionStreak::create(0.22f, 5.0f, 40.0f, Color3B(255, 255, 255), "projects/project1/1_1.png");
    //effectNode->addChild(effectStreak);
    //effectNode->addChild(effectHead);
    //effectStreak->addChild(effectHead);
    addChild(effectStreak);
    addChild(effectHead);
    ccBezierConfig config;
    config.controlPoint_1 = Vec2(300, 100);
    config.controlPoint_2 = Vec2(1000, 0);
    config.endPosition = Vec2(1000, 100);
    
    ccBezierConfig config2;
    config2.controlPoint_1 = Vec2(500, 700);
    config2.controlPoint_2 = config2.controlPoint_1;
    config2.endPosition = Vec2(300, 0);
    
    effectStreak->setPosition(Vec2(300, 0));
    effectHead->setPosition(Vec2(300, 0));
    auto action = RepeatForever::create(
                                        Sequence::create(
                                                         BezierRotateTo::create(1.0f, config),
                                                         MoveBy::create(1.5f, Vec2(0.0f, 800.0f)),

                                                         NULL));
    auto action2 = RepeatForever::create(
                                        Sequence::create(
                                                         BezierTo::create(1.0f, config),
                                                         MoveBy::create(1.5f, Vec2(0.0f, 800.0f)),
                                                         NULL));
    effectStreak->runAction(action2);
    effectHead->runAction(action);
    
}

UI_STATE UILayer::getState()
{
    return mState;
}

void UILayer::onTest1Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        if(!AudioPlayer::getInstance()->isOtherAudioPlaying())
        {
            mStatusText->setString("NONONONO");
        }
        else
        {
            mStatusText->setString("YYYYYYYY");
        }
        //AudioPlayer::getInstance()->playSound(MainScene::getInstance()->getCurrentProject()->getConfig()->projectPath + "haochunguang.mp3");
    }
}

void UILayer::onTest2Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        AudioPlayer::getInstance()->stopOtherAudio();
        AudioPlayer::getInstance()->playSound(MainScene::getInstance()->getCurrentProject()->getConfig()->projectPath + "haochunguang.mp3", 2);
    }
}

void UILayer::onTest3Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        AudioPlayer::getInstance()->stopSound(MainScene::getInstance()->getCurrentProject()->getConfig()->projectPath + "haochunguang.mp3");
    }
}

void UILayer::onPenTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        mState = mState == UI_STATE::PEN ? UI_STATE::NONE : UI_STATE::PEN;
        auto pButton = static_cast<Button*>(sender);
        if(mState == UI_STATE::PEN){
            pButton->setColor(Color3B(200, 200, 255));
            mDrawPanel->setVisible(true);
        }else{
            pButton->setColor(Color3B(255, 255, 255));
            mDrawPanel->setVisible(false);
        }
        
        
    }
    
    
}

void UILayer::onUserTouchEvent(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        
        AudioPlayer::getInstance()->playMusic(MainScene::getInstance()->getCurrentProject()->getConfig()->projectPath + "soda_crush.wav", true);
        //MainLayer::getInstance()->getParticle("3002")->resetSystem();
        
        
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

void UILayer::onSaveTouchEvent(cocos2d::Ref *sender, cocos2d::ui::Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        if(MainScene::getInstance()->getCurrentProject()->saveProject()){
            setStatus("Save Complete!");
        }else{
            setStatus("Save Failed!");
        }
    }
}

void UILayer::setStatus(const std::string &status)
{
    mStatusText->setString(status);
}

void UILayer::loadProject(ProjectConfig* config)
{
    mProjectView->loadProject(config);
}

void UILayer::onSliderMessage1(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_hue", p->value);
}
void UILayer::onSliderMessage2(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_saturation", p->value);
}
void UILayer::onSliderMessage3(void* sender, MessageParam *param)
{
    SliderMessageParam *p = static_cast<SliderMessageParam*>(param);
    MainLayer::getInstance()->getSprite("13")->getGLProgramState()->setUniformFloat("u_value", p->value);
}

NS_EE_END