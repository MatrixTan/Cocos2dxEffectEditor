//
//  ProjectConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "ProjectConfig.hpp"

NS_EE_BEGIN

ProjectConfig::ProjectConfig()
{
}

ProjectConfig::~ProjectConfig()
{
    for(std::vector<SpriteConfig*>::iterator iter = sprites.begin(); iter != sprites.end(); iter++)
    {
        CC_SAFE_DELETE(*iter);
    }
}

NS_EE_END