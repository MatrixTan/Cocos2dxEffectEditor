//
//  Project.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/13.
//
//

#include "Project.hpp"

#include "MainLayer.hpp"
#include "ProjectConfig.hpp"
#include "SpriteConfig.hpp"
#include "Utils.hpp"
#include "PlatformAdapter.h"

NS_EE_BEGIN

Project::Project()
{
}

Project::~Project()
{
    
}

bool Project::load(const std::string& projectPath)
{
    clear();
    mConfig = new ProjectConfig();
    mConfig->loadFile(projectPath);
    return true;
}

bool Project::saveProject()
{
    std::string savePath = PlatformAdapter::getSaveFilePath("json");
    if(savePath.length() > 0){
        std::string formatString = mConfig->getJsonString();
        return FileUtils::getInstance()->writeStringToFile(formatString, savePath);
    }
    return false;
}

ProjectConfig* Project::getConfig()
{
    return mConfig;
}

void Project::clear(){
    if(mConfig != nullptr){
        delete mConfig;
        mConfig = nullptr;
    }
}

void Project::reset(){
    clear();
    mConfig = new ProjectConfig();
}

NS_EE_END
