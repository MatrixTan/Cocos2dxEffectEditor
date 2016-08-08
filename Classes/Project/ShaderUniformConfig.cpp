//
//  ShaderUniformConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "ShaderUniformConfig.hpp"

NS_EE_BEGIN

ShaderUniformConfigTexture::ShaderUniformConfigTexture()
{
    type = SHADER_UNIFORM_TYPE::TEXTURE;
}


ShaderUniformConfigRandom::ShaderUniformConfigRandom()
{
    type = SHADER_UNIFORM_TYPE::RANDOM;
}

ShaderUniformConfigFloat::ShaderUniformConfigFloat()
{
    type = SHADER_UNIFORM_TYPE::FLOAT;
}

ShaderUniformConfigVec4::ShaderUniformConfigVec4()
{
    type = SHADER_UNIFORM_TYPE::VEC4;
}


NS_EE_END