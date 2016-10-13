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
#include "AudioPlayer.hpp"

NS_EE_BEGIN


bool MainScene::init()
{
    mProject = new Project();
    std::string strProject = FileUtils::getInstance()->getWritablePath() + "effect_editor/project2/project.json";
    mProject->init("projects/project1/project1.json");
    
    this->addChild(MainLayer::getInstance(), (int)LAYER_ZORDER::MAIN);
    this->addChild(PostRenderEffectLayer::getInstance(), (int)LAYER_ZORDER::POST_RENDER);
    
    UILayer::getInstance()->loadProject(mProject->getConfig());
    
    AudioPlayer::getInstance()->init();
    
    return Scene::init();
}

Project* MainScene::getCurrentProject()
{
    return mProject;
}

NS_EE_END