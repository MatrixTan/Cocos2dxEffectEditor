//
//  ShaderSprite.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//



#include "ShaderSprite.hpp"

NS_EE_BEGIN


ShaderSprite::ShaderSprite()
:m_startTime(0.0f),
m_programState(nullptr),
m_uniformFlag(0)
{
}

ShaderSprite::~ShaderSprite()
{
}



bool ShaderSprite::init()
{
    initShader("shader/shader_default_vert.glsl", "shader/shader_default_frag.glsl");
    return Sprite::init();
}

bool ShaderSprite::initShader(const std::string& vfile, const std::string& ffile)
{
    cocos2d::GLProgram* program = nullptr;
    program = getProgram(vfile.size()>0?vfile:"shader/shader_default_vert.glsl", ffile.size()>0?ffile:"shader/shader_default_frag.glsl");
    m_programState = cocos2d::GLProgramState::create(program);
    this->setGLProgramState(m_programState);
    m_uniformFlag = 0;
    scheduleUpdate();
    return true;
}

cocos2d::GLProgram* ShaderSprite::getProgram(const std::string& vfile, const std::string& ffile)
{
    auto program = cocos2d::GLProgramCache::getInstance()->getGLProgram(vfile + ffile);
    if(!program)
    {
        program = cocos2d::GLProgram::createWithFilenames(vfile, ffile);
        cocos2d::GLProgramCache::getInstance()->addGLProgram(program, vfile+ffile);
        
    }
    return program;
}

void ShaderSprite::setUniformFlag(uint32_t flag)
{
    m_uniformFlag = flag;
}

void ShaderSprite::draw(cocos2d::Renderer *renderer
                        , const cocos2d::Mat4 &transform
                        , uint32_t flags)
{
    if( m_programState )
    {
        if(m_uniformFlag&(uint32_t)SHADER_UNIFORM_FLAG::TIME)
        {
            m_programState->setUniformFloat("u_time", m_startTime);
        }
        if(m_uniformFlag&(uint32_t)SHADER_UNIFORM_FLAG::LIGHT_DIR)
        {
            m_programState->setUniformVec3("u_light_dir", m_lightPos);
        }
    }
    
    cocos2d::Sprite::draw(renderer, transform, flags);
}

void ShaderSprite::update(float dt)
{
    m_startTime += dt;
    cocos2d::Node::update(dt);
}

NS_EE_END