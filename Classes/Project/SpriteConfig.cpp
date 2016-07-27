//
//  SpriteConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "SpriteConfig.hpp"

NS_EE_BEGIN

SpriteConfig::~SpriteConfig()
{
    for(std::vector<ShaderUniformConfig*>::iterator iter = uniforms.begin(); iter != uniforms.end(); iter++)
    {
        CC_SAFE_DELETE(*iter);
    }
}

SPRITE_SOURCE_TYPE SpriteConfig::getSpriteSouceType(const std::string &type)
{
    if(type == "FILE"){
        return SPRITE_SOURCE_TYPE::FILE;
    }else if(type == "ATLAS"){
        return SPRITE_SOURCE_TYPE::ATLAS;
    }else{
        CCASSERT(false, "error sprite source type");
    }
    return SPRITE_SOURCE_TYPE::NONE;
}

NS_EE_END