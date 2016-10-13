//
//  ParticleConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/1.
//
//

#ifndef ParticleConfig_hpp
#define ParticleConfig_hpp

#include <stdio.h>
#include <string>
#include <cocos2d.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

class ParticleConfig
{
public:
    ParticleConfig();
    
    std::string id;
    std::string file;
    std::string timeline;
    cocos2d::Vec3 position;
    cocos2d::Vec2 scale;
    bool radial;
    
    bool frameTile;
    int tileX;
    int tileY;
    float frameInterval;
    bool randomTile;
};

NS_EE_END


#endif /* ParticleConfig_hpp */
