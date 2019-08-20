//
//  ShaderSprite.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//



#include "ShaderSprite.hpp"
#include "MainScene.hpp"
#include "Project/SpriteConfig.hpp"
#include "Project/ShaderUniformConfig.hpp"

NS_EE_BEGIN


ShaderSprite::ShaderSprite()
:m_startTime(0.0f),
m_programState(nullptr),
m_uniformFlag(0),
m_SelectFlag(nullptr),
m_Config(nullptr)
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

bool ShaderSprite::initConfig(ee::SpriteConfig *config){
    m_Config = config;
    if(config->sourceType == SPRITE_SOURCE_TYPE::FILE){
        this->initWithFile(MainScene::getInstance()->getProjectPath() + config->texture);
    }else if(config->sourceType == SPRITE_SOURCE_TYPE::ATLAS){
        this->initWithSpriteFrameName(config->texture);
    }else{
        CC_ASSERT(false);
    }
    this->initShader(config->vShader, config->fShader);
    Vec2 pos;
    pos.x = config->position.x;
    pos.y = config->position.y;
    int zOrder = (int)config->position.z;
    this->setPosition(pos);
    this->setScale(config->scale.x, config->scale.y);
    this->setLocalZOrder(zOrder);
    if(config->customBlend){
        BlendFunc blendFunc;
        blendFunc.src = config->blendSrc;
        blendFunc.dst = config->blendDst;
        this->setBlendFunc(blendFunc);
    }
    this->setAnchorPoint(config->anchor);
    
    uint32_t uniformFlag = 0;
    for(std::vector<ShaderUniformConfig*>::iterator iterUniform = config->uniforms.begin();
        iterUniform != config->uniforms.end(); iterUniform++)
    {
        if((*iterUniform)->type == SHADER_UNIFORM_TYPE::TIME){
            uniformFlag |= (uint32_t) SHADER_UNIFORM_FLAG::TIME;
        }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::UV_RECT){
            Rect textureRect = this->getSpriteFrame()->getRectInPixels();
            float width = (float)this->getTexture()->getPixelsWide();
            float height = (float)this->getTexture()->getPixelsHigh();
            Vec4 uvRect;
            uvRect.x = textureRect.origin.x / width;
            uvRect.y = (textureRect.origin.x + textureRect.size.width) / width;
            uvRect.z = textureRect.origin.y / height;
            uvRect.w = (textureRect.origin.y + textureRect.size.height) / height;
            this->getGLProgramState()->setUniformVec4("u_uv_rect", uvRect);
        }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::RANDOM){
            auto randomUniform = static_cast<ShaderUniformConfigRandom*>(*iterUniform);
            this->getGLProgramState()->setUniformFloat(randomUniform->name, random<float>(randomUniform->min, randomUniform->max));
        }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::TEXTURE){
            auto textureUniform = static_cast<ShaderUniformConfigTexture*>(*iterUniform);
            Texture2D* texture = nullptr;
            if(textureUniform->sourceType == SPRITE_SOURCE_TYPE::FILE){
                texture = Director::getInstance()->getTextureCache()->addImage(MainScene::getInstance()->getProjectPath() + textureUniform->texture);
            }else if(textureUniform->sourceType == SPRITE_SOURCE_TYPE::ATLAS){
                texture = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureUniform->texture)->getTexture();
            }
            this->getGLProgramState()->setUniformTexture(textureUniform->name, texture);
        }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::FLOAT){
            auto floatUniform = static_cast<ShaderUniformConfigFloat*>(*iterUniform);
            this->getGLProgramState()->setUniformFloat(floatUniform->name, floatUniform->value);
        }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::VEC4){
            auto vec4Uniform = static_cast<ShaderUniformConfigVec4*>(*iterUniform);
            this->getGLProgramState()->setUniformVec4(vec4Uniform->name, vec4Uniform->value);
        }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::VEC2){
            auto vec2Uniform = static_cast<ShaderUniformConfigVec2*>(*iterUniform);
            this->getGLProgramState()->setUniformVec2(vec2Uniform->name, vec2Uniform->value);
        }
    }
    this->setUniformFlag(uniformFlag);
    this->setVisible(config->visible);
    this->setOpacity(255 * config->alpha);
    this->setRotation(config->rotation);
    return true;
}

bool ShaderSprite::initShader(const std::string& vfile, const std::string& ffile)
{
    cocos2d::GLProgram* program = nullptr;
    program = getProgram(vfile, ffile);
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
    
    if(m_SelectFlag != nullptr){
        m_SelectFlag->clear();
        auto count = _polyInfo.triangles.indexCount/3;
        auto indices = _polyInfo.triangles.indices;
        auto verts = _polyInfo.triangles.verts;
        for(ssize_t i = 0; i < count; i++)
        {
            //draw 3 lines
            Vec3 from =verts[indices[i*3]].vertices;
            Vec3 to = verts[indices[i*3+1]].vertices;
            m_SelectFlag->drawLine(Vec2(from.x, from.y), Vec2(to.x,to.y), Color4F::WHITE);
            
            from =verts[indices[i*3+1]].vertices;
            to = verts[indices[i*3+2]].vertices;
            m_SelectFlag->drawLine(Vec2(from.x, from.y), Vec2(to.x,to.y), Color4F::WHITE);
            
            from =verts[indices[i*3+2]].vertices;
            to = verts[indices[i*3]].vertices;
            m_SelectFlag->drawLine(Vec2(from.x, from.y), Vec2(to.x,to.y), Color4F::WHITE);
        }

    }
}

void ShaderSprite::update(float dt)
{
    m_startTime += dt;
    cocos2d::Node::update(dt);
}

void ShaderSprite::resetTimeUniform()
{
    m_startTime = 0.0f;
}

void ShaderSprite::enableSelect(bool enable){
    if(enable){
        if(m_SelectFlag == nullptr){
            m_SelectFlag = DrawNode::create();
            addChild(m_SelectFlag);
        }
    }else{
        if(m_SelectFlag != nullptr){
            m_SelectFlag->removeFromParent();
            m_SelectFlag = nullptr;
        }
    }
}

NS_EE_END
