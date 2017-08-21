//
//  ProjectConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#ifndef ProjectConfig_hpp
#define ProjectConfig_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "GlobalDefine.hpp"
#include "BackgroundConfig.hpp"
#include "ParticleConfig.hpp"
#include "TimelineConfig.hpp"
#include "AnimationConfig.hpp"
#include "MaskConfig.hpp"
#include "Timeline.hpp"
#include <json/rapidjson.h>
#include <json/document.h>
#include <json/writer.h>
#include <json/stringbuffer.h>

NS_EE_BEGIN

class SpriteConfig;

class ProjectConfig
{
public:
    
    ProjectConfig();
    ~ProjectConfig();
    
    bool loadFile(const std::string& file);
    std::string getJsonString() const;
    
    std::string version;
    BackgroundConfig background;
    std::string projectPath;
    std::string projectFilePath;
    std::vector<std::string> atlas;
    std::map<std::string, SpriteConfig*> sprites;
    std::map<std::string, ParticleConfig*> particles;
    std::map<std::string, Timeline*> timelines;
    std::map<std::string, AnimationConfig*> animations;
    std::map<std::string, MaskConfig*> masks;
    
private:
    Timeline* parseTimeline(const rapidjson::Value& value);
    
    
};

NS_EE_END

#endif /* ProjectConfig_hpp */
