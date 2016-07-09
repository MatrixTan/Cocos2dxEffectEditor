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

NS_EE_BEGIN

MainLayer::MainLayer()
:mContainer(nullptr)
{
}

MainLayer::~MainLayer()
{

}

bool MainLayer::init()
{
    FileUtils::getInstance()->addSearchPath("res/");
    
    mContainer = CSLoader::createNode("res/main_scene.csb");
    auto size = Director::getInstance()->getWinSize();
    this->addChild(mContainer);
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/item/candy_patch.plist");
    for(int i=0; i<30; i++){
        auto *sprite = Sprite::create();
        sprite->initWithSpriteFrameName(StringUtils::format("item_%d.png", i % 8 + 1));
        sprite->setPosition(cocos2d::random<float>(-300, 300), cocos2d::random<float>(-300.0f, 300.0f));
        auto move = MoveBy::create(3.0f, Vec2(cocos2d::random<float>(-200, 200), cocos2d::random<float>(-200.0f, 200.0f)));
        sprite->runAction(Repeat::create(Sequence::create(move, move->reverse(),NULL), INT_MAX));
        mContainer->getChildByName("root_node")->addChild(sprite);
    }


    return Layer::init();
}

void MainLayer::setSprite(const std::string &file)
{
    if(mContainer)
    {
        ShaderSprite *sprite = ShaderSprite::create();
        sprite->initWithFile(file);
        mContainer->getChildByName("root_node")->addChild(sprite);
    }
}

NS_EE_END