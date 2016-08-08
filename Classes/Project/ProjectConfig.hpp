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
#include "GlobalDefine.hpp"
#include "BackgroundConfig.hpp"
#include "ParticleConfig.hpp"
#include "TimelineConfig.hpp"
#include "AnimationConfig.cpp"

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
    std::vector<std::string> atlas;
    std::vector<SpriteConfig*> sprites;
    std::vector<ParticleConfig*> particles;
    std::map<std::string, Timeline*> timelines;
    std::map<std::string, AnimationConfig*> animations;
};

NS_EE_END

#endif /* ProjectConfig_hpp */