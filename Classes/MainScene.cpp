//
//  MainScene.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#include "MainScene.hpp"

NS_EE_BEGIN


bool MainScene::init()
{
    MainLayer *layer = MainLayer::create();
    this->addChild(layer);
    
    return Scene::init();
}

NS_EE_END