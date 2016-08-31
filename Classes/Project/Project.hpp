//
//  Project.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/13.
//
//

#ifndef Project_hpp
#define Project_hpp

#include <stdio.h>
#include <json/rapidjson.h>
#include <json/document.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "ProjectConfig.hpp"
#include "TimelineConfig.hpp";

NS_EE_BEGIN

using namespace cocos2d;

class Project
{
public:
    Project();
    ~Project();
    
    bool init(const std::string& projectPath);
    
    bool saveProject();
    ProjectConfig* getConfig();
    
private:
    Timeline* parseTimeline(const rapidjson::Value& value);
    void loadProject(void);    
    
    ProjectConfig mConfig;
    rapidjson::Document mRoot;
};

NS_EE_END

#endif /* Project_hpp */
