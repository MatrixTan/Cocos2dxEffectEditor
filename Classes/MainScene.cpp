//
//  MainScene.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#include "MainScene.hpp"
#include "PostRenderEffectLayer.hpp"


NS_EE_BEGIN


bool MainScene::init()
{
    this->addChild(MainLayer::getInstance());
    this->addChild(PostRenderEffectLayer::getInstance(), 100);
    //PostRenderEffectLayer::getInstance()->setDrawRect(Rect(100, 100, 500, 500));
    return Scene::init();
}

NS_EE_END