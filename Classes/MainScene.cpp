//
//  MainScene.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#include "MainScene.hpp"
#include "PostRenderEffectLayer.hpp"
#include "Project.hpp"

NS_EE_BEGIN


bool MainScene::init()
{
    auto p = new Project();
    p->init("projects/project2/project.json");
    
    this->addChild(MainLayer::getInstance(), (int)LAYER_ZORDER::MAIN);
    this->addChild(PostRenderEffectLayer::getInstance(), (int)LAYER_ZORDER::POST_RENDER);
    return Scene::init();
}

NS_EE_END