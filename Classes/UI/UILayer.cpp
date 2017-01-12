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
#include "BezierPathManager.hpp"
#include "ParticleSystemExt.hpp"
#include "PlatformAdapter.h"
#include "ActionDrawBezierPath.hpp"
#include "TimelineManager.hpp"

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
    
}

UI_STATE UILayer::getState()
{
    return mState;
}

void UILayer::onTest1Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        for(int i=0; i<5; i++){
            auto from = Vec2(random(400.0f, 878.0f), random(78.0f, 721.0f));
            auto to = Vec2(random(400.0f, 878.0f), random(78.0f, 721.0f));
            auto effectHead = Sprite::create("projects/project1/2.png");
            effectHead->setAnchorPoint(Vec2(0.5f, 0.0f));
            auto effectStreak = MotionStreak::create(0.22f, 5.0f, 40.0f, Color3B(255, 255, 255), "projects/project1/1_1.png");
            addChild(effectStreak);
            addChild(effectHead);
            
            effectStreak->setPosition(from);
            effectHead->setPosition(from);
            
            std::string bezierName = "";
            if(from.distance(to) > 210.0f){
                bezierName = "f";
            }else{
                bezierName = "n";
            }
            int randomPath = random(1, 5);
            bezierName += std::to_string(randomPath);
            bezierName += ".bezier";
            CCLOG("%s",bezierName.c_str());
            
            auto action3 = BezierPathManager::getInstance()->getBezierPathSequence(FileUtils::getInstance()->getWritablePath() + bezierName, 1.5f, from, to, true);
            auto action4 = BezierPathManager::getInstance()->getBezierPathSequence(FileUtils::getInstance()->getWritablePath() + bezierName, 1.5f, from, to);
            
            auto callfunc1 = [&, to](){
                auto particle = ParticleSystemExt::create("projects/project1/test.plist");
                particle->setPosition(to);
                particle->setAutoRemoveOnFinish(true);
                addChild(particle);
            };
            
            effectStreak->runAction(Sequence::create(action4, RemoveSelf::create(), CallFunc::create(callfunc1), NULL));
            effectHead->runAction(Sequence::create(action3, RemoveSelf::create(),  NULL));

        }
        
    }
}

void UILayer::onTest2Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto maskSprite = DrawNode::create();
        auto clipingNode = ClippingNode::create(maskSprite);
        auto show = Sprite::create("projects/project1/monster.png");
        //show->setPosition(Vec2(0.0f, -show->getContentSize().height/2));
        show->setAnchorPoint(Vec2(0.0f, 0.0f));
        //show->setScale(3.0f);
        clipingNode->addChild(show);
        clipingNode->setPosition(Vec2(300, 300));
        addChild(clipingNode, (int)SPRITE_ZORDER::MASK);
        maskSprite->drawDot(Vec2(0, 0), 10, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
        
        BezierPointList list = BezierPathManager::getInstance()->getBezierPath("projects/project1/monster2.bezier");
        maskSprite->runAction(ActionDrawBezierPath::create(5.0f, list, 10.0f));
    }
    
}

void UILayer::onTest3Event(cocos2d::Ref *sender, Widget::TouchEventType type)
{
    if(type == Widget::TouchEventType::ENDED)
    {
        auto sprite = MainLayer::getInstance()->getSprite("5");
        sprite->resetTimeUniform();
        /*Timeline* timeline = TimelineManager::getInstance()->getTimelineFromFile(
                                                                  FileUtils::getInstance()->getWritablePath()
                                                                  + "effect_editor/project2/magic_word2.timeline");
        
        sprite->runAction(timeline->getAction());
         */
        
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
