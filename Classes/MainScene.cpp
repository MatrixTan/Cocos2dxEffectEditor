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
    std::string strProject = FileUtils::getInstance()->getWritablePath() + "effect_editor/project1/project1.json";
    //std::string strProject = "projects/project1/project1.json";
    mProject->init(strProject);
    
    this->addChild(MainLayer::getInstance(), (int)LAYER_ZORDER::MAIN);
    MainLayer::getInstance()->addChild(UILayer::getInstance(), (int)SPRITE_ZORDER::UI);
    this->addChild(PostRenderEffectLayer::getInstance(), (int)LAYER_ZORDER::POST_RENDER);
    
    UILayer::getInstance()->loadProject(mProject->getConfig());
    
    AudioPlayer::getInstance()->init();
    
    return Scene::init();
}

Project* MainScene::getCurrentProject()
{
    return mProject;
}

std::string MainScene::getProjectPath(){
    if(mProject != nullptr){
        return mProject->getConfig()->projectPath;
    }
    return "";
}

NS_EE_END
