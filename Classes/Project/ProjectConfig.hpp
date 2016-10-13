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

NS_EE_BEGIN

class SpriteConfig;

class ProjectConfig
{
public:
    
    ProjectConfig();
    ~ProjectConfig();
    
    std::string version;
    BackgroundConfig background;
    std::string projectPath;
    std::string projectFilePath;
    std::string projectString;
    std::vector<std::string> atlas;
    std::vector<SpriteConfig*> sprites;
    std::vector<ParticleConfig*> particles;
    std::map<std::string, Timeline*> timelines;
    std::map<std::string, AnimationConfig*> animations;
    std::map<std::string, MaskConfig*> masks;
};

NS_EE_END

#endif /* ProjectConfig_hpp */