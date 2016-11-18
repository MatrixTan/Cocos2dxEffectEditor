//
//  SpriteConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "SpriteConfig.hpp"

NS_EE_BEGIN

SpriteConfig::SpriteConfig()
{
    scale = Vec2(1.0f, 1.0f);
    anchor = Vec2(0.5f, 0.5f);
    customBlend = false;
    alpha = 1.0f;
    rotation = 0.0f;
}

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

void SpriteConfig::setBlendFun(const std::string &src, const std::string &dst)
{
    customBlend = true;
    blendSrc = getBlendFunc(src);
    blendDst = getBlendFunc(dst);
}

GLenum SpriteConfig::getBlendFunc(const std::string &name)
{
    if(name == "ZERO"){
        return GL_ZERO;
    }else if(name == "ONE"){
        return GL_ONE;
    }else if(name == "SRC_COLOR"){
        return GL_SRC_COLOR;
    }else if(name == "ONE_MINUS_SRC_COLOR"){
        return GL_ONE_MINUS_SRC_COLOR;
    }else if(name == "DST_COLOR"){
        return GL_DST_COLOR;
    }else if(name == "ONE_MINUS_DST_COLOR"){
        return GL_ONE_MINUS_DST_COLOR;
    }else if(name == "SRC_ALPHA"){
        return GL_SRC_ALPHA;
    }else if(name == "ONE_MINUS_SRC_ALPHA"){
        return GL_ONE_MINUS_SRC_ALPHA;
    }else if(name == "DST_ALPHA"){
        return GL_DST_ALPHA;
    }else if(name == "ONE_MINUS_DST_ALPHA"){
        return GL_ONE_MINUS_DST_ALPHA;
    }
    return 0;
}

NS_EE_END