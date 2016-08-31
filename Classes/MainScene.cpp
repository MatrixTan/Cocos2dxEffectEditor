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
#include "UILayer.hpp"

NS_EE_BEGIN


bool MainScene::init()
{
    mProject = new Project();
    std::string strProject = FileUtils::getInstance()->getWritablePath() + "effect_editor/project2/project.json";
    mProject->init(strProject);
    
    this->addChild(MainLayer::getInstance(), (int)LAYER_ZORDER::MAIN);
    this->addChild(PostRenderEffectLayer::getInstance(), (int)LAYER_ZORDER::POST_RENDER);
    
    UILayer::getInstance()->loadProject(mProject->getConfig());
    return Scene::init();
}

Project* MainScene::getCurrentProject()
{
    return mProject;
}

NS_EE_END