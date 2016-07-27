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
    RANDOM
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
    std::string name;
    std::string texture;
    SPRITE_SOURCE_TYPE sourceType;
};

class ShaderUniformConfigRandom : public ShaderUniformConfig
{
public:
    std::string name;
    float min;
    float max;
};

NS_EE_END

#endif /* ShaderUniformConfig_hpp */
