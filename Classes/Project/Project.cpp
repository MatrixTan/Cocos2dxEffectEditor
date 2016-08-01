//
//  Project.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/13.
//
//

#include "Project.hpp"
#include <json/document.h>
#include "MainLayer.hpp"
#include "ProjectConfig.hpp"
#include "SpriteConfig.hpp"
#include "ShaderUniformConfig.hpp"
#include "ParticleConfig.hpp"

NS_EE_BEGIN

Project::Project()
{
}

Project::~Project()
{
}

bool Project::init(const std::string& projectPath)
{
    std::string strProject = FileUtils::getInstance()->getStringFromFile(projectPath);
    std::string fullPath = FileUtils::getInstance()->fullPathForFilename(projectPath);
    std::string directoryPath = fullPath.substr(0, fullPath.find_last_of('/')) + "/";
    rapidjson::Document root;
    root.Parse<0>(strProject.c_str());
    if(root.HasParseError()){
        CCASSERT(false, "project parse error");
        return false;
    }
    
    mConfig.projectPath = directoryPath;
    mConfig.version = root["version"].GetString();
    
    if(root["background"].IsObject()){
        mConfig.background.file = root["background"]["file"].GetString();
        mConfig.background.scale.x = root["background"]["scale"]["x"].GetDouble();
        mConfig.background.scale.y = root["background"]["scale"]["y"].GetDouble();
    }
    
    rapidjson::Value &atlasArray = root["atlas"];
    for(int i=0; i<atlasArray.Size(); i++){
        mConfig.atlas.push_back(atlasArray[i].GetString());
    }
    
    rapidjson::Value &sprites = root["sprites"];
    for(int i=0; i<sprites.Size(); i++){
        auto spriteConfig = new(std::nothrow)SpriteConfig();
        spriteConfig->id = sprites[i]["id"].GetString();
        spriteConfig->texture = sprites[i]["texture"].GetString();
        spriteConfig->visible = sprites[i]["visible"].GetBool();
        std::string sourceType = sprites[i]["source_type"].GetString();
        spriteConfig->sourceType = SpriteConfig::getSpriteSouceType(sourceType);

        if(sprites[i].HasMember("v_shader")){
            spriteConfig->vShader = mConfig.projectPath + sprites[i]["v_shader"].GetString();
        }else{
            spriteConfig->vShader = "shader/shader_default_vert.glsl";
        }

        if(sprites[i].HasMember("f_shader")){
            spriteConfig->fShader = mConfig.projectPath + sprites[i]["f_shader"].GetString();
        }else{
            spriteConfig->fShader = "shader/shader_default_frag.glsl";
        }
        
        rapidjson::Value &position = sprites[i]["pos"];
        
        spriteConfig->position.x = position["x"].GetDouble();
        spriteConfig->position.y = position["y"].GetDouble();
        spriteConfig->position.z = position["z"].GetDouble();
        
        if(sprites[i].HasMember("scale")){
            rapidjson::Value &scale = sprites[i]["scale"];
            spriteConfig->scale.x = scale["x"].GetDouble();
            spriteConfig->scale.y = scale["y"].GetDouble();
        }
        
        if(sprites[i].HasMember("uniform")){
            rapidjson::Value &uniform = sprites[i]["uniform"];
            for(int j=0; j<uniform.Size(); j++){
                std::string uniformType = uniform[j]["type"].GetString();
                if(uniformType == "TEXTURE"){
                    auto uniformConfigTexture = new(std::nothrow)ShaderUniformConfigTexture();
                    uniformConfigTexture->type = SHADER_UNIFORM_TYPE::TEXTURE;
                    uniformConfigTexture->name = uniform[j]["name"].GetString();
                    uniformConfigTexture->texture = uniform[j]["texture"].GetString();
                    uniformConfigTexture->sourceType = SpriteConfig::getSpriteSouceType(uniform[j]["source_type"].GetString());
                    spriteConfig->uniforms.push_back(uniformConfigTexture);
                    
                }else if(uniformType == "TIME"){
                    auto uniformConfig = new(std::nothrow)ShaderUniformConfig();
                    uniformConfig->type = SHADER_UNIFORM_TYPE::TIME;
                    spriteConfig->uniforms.push_back(uniformConfig);
                }else if(uniformType == "RANDOM"){
                    auto uniformConfigRandom = new(std::nothrow)ShaderUniformConfigRandom();
                    uniformConfigRandom->type = SHADER_UNIFORM_TYPE::RANDOM;
                    uniformConfigRandom->name = uniform[j]["name"].GetString();
                    uniformConfigRandom->min = uniform[j]["min"].GetDouble();
                    uniformConfigRandom->max = uniform[j]["max"].GetDouble();
                    spriteConfig->uniforms.push_back(uniformConfigRandom);
                }else if(uniformType == "UV_RECT"){
                    auto uniformConfig = new(std::nothrow)ShaderUniformConfig();
                    uniformConfig->type = SHADER_UNIFORM_TYPE::UV_RECT;
                    spriteConfig->uniforms.push_back(uniformConfig);
                }
            }
        }
        
        if(sprites[i].HasMember("timeline")){
            spriteConfig->timeline = sprites[i]["timeline"].GetString();
        }
        
        mConfig.sprites.push_back(spriteConfig);
    }
    
    rapidjson::Value& particles = root["particles"];
    if(!particles.IsNull()){
        for(int i=0;i<particles.Size();i++){
            rapidjson::Value& particle = particles[i];
            auto pParticle = new(std::nothrow) ParticleConfig();
            pParticle->id = particle["id"].GetString();
            pParticle->file = particle["file"].GetString();
            if(particle.HasMember("timeline")){
                pParticle->timeline = particle["timeline"].GetString();
            }
            rapidjson::Value& particlePos = particle["pos"];
            pParticle->position.x = particlePos["x"].GetDouble();
            pParticle->position.y = particlePos["y"].GetDouble();
            pParticle->position.z = particlePos["z"].GetDouble();
            mConfig.particles.push_back(pParticle);
        }
    }
    
    rapidjson::Value& timelines = root["timelines"];
    if(!timelines.IsNull()){
        for(int i=0; i<timelines.Size(); i++){
            rapidjson::Value& timeline = timelines[i];
            std::string timeId = timeline["id"].GetString();
            mConfig.timelines[timeId] = parseTimeline(timeline);
        }
        
    }
    
    loadProject();
    return true;
}

Timeline* Project::parseTimeline(const rapidjson::Value &value)
{
    std::string type = value["type"].GetString();
    if(type == "SEQUENCE"){
        auto sequenceTimeline = new(std::nothrow) TimelineSequence();
        const rapidjson::Value& children = value["children"];
        for(int i=0; i<children.Size(); i++){
            sequenceTimeline->children.push_back(parseTimeline(children[i]));
        }
        return sequenceTimeline;
    }else if(type == "MOVE_BY"){
        auto moveByTimeline = new(std::nothrow) TimelineMoveBy();
        moveByTimeline->duration = value["duration"].GetDouble();
        moveByTimeline->x = value["x"].GetDouble();
        moveByTimeline->y = value["y"].GetDouble();
        return moveByTimeline;
    }
    return nullptr;
}

void Project::loadProject()
{
    MainLayer::getInstance()->setBackground(mConfig.projectPath + mConfig.background.file, mConfig.background.scale);
    for(std::vector<std::string>::iterator iter = mConfig.atlas.begin(); iter != mConfig.atlas.end(); iter++)
    {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(mConfig.projectPath + *iter);
    }
    auto spriteOrigin = Director::getInstance()->getWinSize() * 0.5f;
    
    for(std::vector<SpriteConfig*>::iterator iter = mConfig.sprites.begin(); iter != mConfig.sprites.end(); iter++)
    {
        auto shaderSprite = ShaderSprite::create();
        if((*iter)->sourceType == SPRITE_SOURCE_TYPE::FILE){
            shaderSprite->initWithFile(mConfig.projectPath + (*iter)->texture);
        }else if((*iter)->sourceType == SPRITE_SOURCE_TYPE::ATLAS){
            shaderSprite->initWithSpriteFrameName((*iter)->texture);
        }else{
            CC_ASSERT(false);
        }
        shaderSprite->initShader((*iter)->vShader, (*iter)->fShader);
        Vec2 pos;
        pos.x = (*iter)->position.x + spriteOrigin.width;
        pos.y = (*iter)->position.y + spriteOrigin.height;
        int zOrder = (int)(*iter)->position.z;
        shaderSprite->setPosition(pos);
        shaderSprite->setScale((*iter)->scale.x, (*iter)->scale.y);
        shaderSprite->setLocalZOrder(zOrder);
        
        uint32_t uniformFlag = 0;
        for(std::vector<ShaderUniformConfig*>::iterator iterUniform = (*iter)->uniforms.begin();
            iterUniform != (*iter)->uniforms.end(); iterUniform++)
        {
            if((*iterUniform)->type == SHADER_UNIFORM_TYPE::TIME){
                uniformFlag |= (uint32_t) SHADER_UNIFORM_FLAG::TIME;
            }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::UV_RECT){
                Rect textureRect = shaderSprite->getSpriteFrame()->getRectInPixels();
                float width = (float)shaderSprite->getTexture()->getPixelsWide();
                float height = (float)shaderSprite->getTexture()->getPixelsHigh();
                Vec4 uvRect;
                uvRect.x = textureRect.origin.x / width;
                uvRect.y = (textureRect.origin.x + textureRect.size.width) / width;
                uvRect.z = textureRect.origin.y / height;
                uvRect.w = (textureRect.origin.y + textureRect.size.height) / height;
                shaderSprite->getGLProgramState()->setUniformVec4("u_uv_rect", uvRect);
            }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::RANDOM){
                auto randomUniform = static_cast<ShaderUniformConfigRandom*>(*iterUniform);
                shaderSprite->getGLProgramState()->setUniformFloat(randomUniform->name, random<float>(randomUniform->min, randomUniform->max));
            }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::TEXTURE){
                auto textureUniform = static_cast<ShaderUniformConfigTexture*>(*iterUniform);
                Texture2D* texture = nullptr;
                if(textureUniform->sourceType == SPRITE_SOURCE_TYPE::FILE){
                    texture = Director::getInstance()->getTextureCache()->addImage(mConfig.projectPath + textureUniform->texture);
                }else if(textureUniform->sourceType == SPRITE_SOURCE_TYPE::ATLAS){
                    texture = SpriteFrameCache::getInstance()->getSpriteFrameByName(textureUniform->texture)->getTexture();
                }
                shaderSprite->getGLProgramState()->setUniformTexture(textureUniform->name, texture);
            }
        }
        shaderSprite->setUniformFlag(uniformFlag);
        shaderSprite->setVisible((*iter)->visible);
        if((*iter)->timeline.length() > 0){
            shaderSprite->runAction(mConfig.timelines[(*iter)->timeline]->getAction());
        }
        
        MainLayer::getInstance()->addSprite((*iter)->id, shaderSprite, zOrder);
    }
    
    for(std::vector<ParticleConfig*>::iterator iter = mConfig.particles.begin();
        iter != mConfig.particles.end();
        iter++){
        auto particle = ParticleSystemQuad::create(mConfig.projectPath + (*iter)->file);
        particle->setPosition((*iter)->position.x + spriteOrigin.width
                              , (*iter)->position.y+spriteOrigin.height);
        if((*iter)->timeline.length() > 0){
            particle->runAction(mConfig.timelines[(*iter)->timeline]->getAction());
        }
        MainLayer::getInstance()->addParticleSystem((*iter)->id, particle, (*iter)->position.z);
    }

}

NS_EE_END