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