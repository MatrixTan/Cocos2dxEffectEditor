//
//  ShaderUniformConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#ifndef ShaderUniformConfig_hpp
#define ShaderUniformConfig_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

enum class SHADER_UNIFORM_TYPE
{
    TIME,
    TEXTURE,
    UV_RECT,
    RANDOM,
    FLOAT,
    VEC4
};

class ShaderUniformConfig
{
public:
    SHADER_UNIFORM_TYPE type;
};


enum class SPRITE_SOURCE_TYPE;

class ShaderUniformConfigTexture : public ShaderUniformConfig
{
public:
    ShaderUniformConfigTexture();
    std::string name;
    std::string texture;
    SPRITE_SOURCE_TYPE sourceType;
};

class ShaderUniformConfigRandom : public ShaderUniformConfig
{
public:
    ShaderUniformConfigRandom();
    std::string name;
    float min;
    float max;
};

class ShaderUniformConfigFloat : public ShaderUniformConfig
{
public:
    ShaderUniformConfigFloat();
    std::string name;
    float value;
};

class ShaderUniformConfigVec4 : public ShaderUniformConfig
{
public:
    ShaderUniformConfigVec4();
    std::string name;
    cocos2d::Vec4 value;
};

NS_EE_END

#endif /* ShaderUniformConfig_hpp */
