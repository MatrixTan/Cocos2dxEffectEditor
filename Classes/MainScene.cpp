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
   
    return Scene::init();
}

Project* MainScene::getCurrentProject()
{
    return mProject;
}

void MainScene::initScene(){
    AudioPlayer::getInstance()->init();
    
    this->addChild(MainLayer::getInstance(), (int)LAYER_ZORDER::MAIN);
    MainLayer::getInstance()->addChild(UILayer::getInstance(), (int)SPRITE_ZORDER::UI);
    this->addChild(PostRenderEffectLayer::getInstance(), (int)LAYER_ZORDER::POST_RENDER);
}

std::string MainScene::getProjectPath(){
    if(mProject != nullptr){
        return mProject->getConfig()->projectPath;
    }
    return "";
}

bool MainScene::loadProject(const std::string &path){
    if(mProject != nullptr){
        delete mProject;
        mProject = nullptr;
    }
    mProject = new Project();
    if(mProject->init(path)){
        MainLayer::getInstance()->loadProject(mProject);
        return true;
    }
    return false;
}

void MainScene::newProject(){
    if(mProject != nullptr){
        delete mProject;
        mProject = nullptr;
    }
    mProject = new Project();
    MainLayer::getInstance()->clear();
}

NS_EE_END
