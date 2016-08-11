//
//  MainLayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#include "MainLayer.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ShaderSprite.hpp"
#include "PostRenderEffectLayer.hpp"
#include "UILayer.hpp"

NS_EE_BEGIN

MainLayer::MainLayer()
{
}

MainLayer::~MainLayer()
{

}

bool MainLayer::init()
{
    
    FileUtils::getInstance()->addSearchPath("res/");
    
    addChild(UILayer::getInstance(), (int)SPRITE_ZORDER::UI);
    
    /*auto test = CSLoader::createNode("res/horizontal_dispose.csb");
    cocostudio::timeline::ActionTimeline *timeline = CSLoader::createTimeline("res/horizontal_dispose.csb");
    test->runAction(timeline);
    timeline->gotoFrameAndPlay(0);
    timeline->addFrameEndCallFunc(40, "timeend", [test](){
        //test->removeFromParent();
    });
    this->addChild(test, 999999);
    test->setPosition(Vec2(300, 300));
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/item/candy_patch.plist");
    for(int i=0; i<30; i++){
        auto *sprite = Sprite::create();
        sprite->initWithSpriteFrameName(StringUtils::format("item_%d.png", i % 8 + 1));
        sprite->setPosition(cocos2d::random<float>(-300, 300), cocos2d::random<float>(-300.0f, 300.0f));
        auto move = MoveBy::create(3.0f, Vec2(cocos2d::random<float>(-200, 200), cocos2d::random<float>(-200.0f, 200.0f)));
        sprite->runAction(Repeat::create(Sequence::create(move, move->reverse(),NULL), INT_MAX));
        mContainer->getChildByName("root_node")->addChild(sprite);
    }*/
    
    
    
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegin, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMove, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnd, this);
    touchEventListener->onTouchCancelled = CC_CALLBACK_2(MainLayer::onTouchCancel, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    return Layer::init();
}


void MainLayer::setBackground(const std::string &file, const Vec2& scale)
{
    auto background = Sprite::create(file);
    background->setPosition(Director::getInstance()->getWinSize() * 0.5f);
    background->setScale(scale.x, scale.y);
    addChild(background);
}

void MainLayer::addSprite(const std::string& id, ShaderSprite* pSprite, int zorder)
{
    addChild(pSprite, (int)SPRITE_ZORDER::SPRITE + zorder);
    pSprite->retain();
    mSprites.insert(std::pair<std::string, ShaderSprite*>(id, pSprite));
}

ShaderSprite* MainLayer::getSprite(const std::string& id)
{
    std::map<std::string, ShaderSprite*>::iterator iter = mSprites.find(id);
    if(iter != mSprites.end())
    {
        return iter->second;
    }
    return nullptr;
}

void MainLayer::addParticleSystem(const std::string &id, cocos2d::ParticleSystemQuad *particle, int zorder)
{
    addChild(particle, (int)SPRITE_ZORDER::SPRITE + zorder);
    particle->retain();
    mParticles.insert(std::pair<std::string, ParticleSystemQuad*>(id, particle));
}

ParticleSystemQuad* MainLayer::getParticle(const std::string &id)
{
    std::map<std::string, ParticleSystemQuad*>::iterator iter = mParticles.find(id);
    if(iter != mParticles.end())
    {
        return iter->second;
    }
    return nullptr;
}

bool MainLayer::onTouchBegin(cocos2d::Touch *touch, cocos2d::Event *event)
{
    return true;
}

void MainLayer::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event)
{
    
}

void MainLayer::onTouchEnd(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto pos = touch->getLocation();
    //PostRenderEffectLayer::getInstance()->setDrawRect(Rect(pos.x - 300, pos.y - 300, 600, 600), 0.70f);
}

void MainLayer::onTouchCancel(Touch *touch, Event *event)
{
    
}

NS_EE_END