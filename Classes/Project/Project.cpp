//
//  Project.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/13.
//
//

#include "Project.hpp"
#include <json/document.h>
#include <json/writer.h>
#include <json/stringbuffer.h>
#include "MainLayer.hpp"
#include "ProjectConfig.hpp"
#include "SpriteConfig.hpp"
#include "ShaderUniformConfig.hpp"
#include "ParticleConfig.hpp"
#include "AnimationConfig.hpp"
#include "Utils.hpp"
#include "ParticleSystemExt.hpp"
#include "MaskConfig.hpp"

NS_EE_BEGIN

Project::Project()
{
}

Project::~Project()
{
}

bool Project::init(const std::string& projectPath)
{
    mConfig.projectString = FileUtils::getInstance()->getStringFromFile(projectPath);
    mConfig.projectFilePath = FileUtils::getInstance()->fullPathForFilename(projectPath);
    mConfig.projectPath = mConfig.projectFilePath.substr(0, mConfig.projectFilePath.find_last_of('/')) + "/";
    mRoot.Parse<0>(mConfig.projectString.c_str());
    if(mRoot.HasParseError()){
        CCASSERT(false, "project parse error");
        return false;
    }
    

    mConfig.version = mRoot["version"].GetString();
    
    if(mRoot.HasMember("background")){
        mConfig.background.file = mRoot["background"]["file"].GetString();
        mConfig.background.scale.x = mRoot["background"]["scale"]["x"].GetDouble();
        mConfig.background.scale.y = mRoot["background"]["scale"]["y"].GetDouble();
    }
    
    if(mRoot.HasMember("atlas")){
        rapidjson::Value &atlasArray = mRoot["atlas"];
        for(int i=0; i<atlasArray.Size(); i++){
            mConfig.atlas.push_back(atlasArray[i].GetString());
        }
    }
    
    if(mRoot.HasMember("sprites")){
        rapidjson::Value &sprites = mRoot["sprites"];
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
                        uniformConfigRandom->name = uniform[j]["name"].GetString();
                        uniformConfigRandom->min = uniform[j]["min"].GetDouble();
                        uniformConfigRandom->max = uniform[j]["max"].GetDouble();
                        spriteConfig->uniforms.push_back(uniformConfigRandom);
                    }else if(uniformType == "UV_RECT"){
                        auto uniformConfig = new(std::nothrow)ShaderUniformConfig();
                        uniformConfig->type = SHADER_UNIFORM_TYPE::UV_RECT;
                        spriteConfig->uniforms.push_back(uniformConfig);
                    }else if(uniformType == "FLOAT"){
                        auto uniformConfig = new(std::nothrow)ShaderUniformConfigFloat();
                        uniformConfig->name = uniform[j]["name"].GetString();
                        uniformConfig->value = uniform[j]["value"].GetDouble();
                        spriteConfig->uniforms.push_back(uniformConfig);
                    }else if(uniformType == "VEC4"){
                        auto uniformConfig = new(std::nothrow)ShaderUniformConfigVec4();
                        uniformConfig->name = uniform[j]["name"].GetString();
                        uniformConfig->value.x = uniform[j]["value"]["x"].GetDouble();
                        uniformConfig->value.y = uniform[j]["value"]["y"].GetDouble();
                        uniformConfig->value.z = uniform[j]["value"]["z"].GetDouble();
                        uniformConfig->value.w = uniform[j]["value"]["w"].GetDouble();
                        spriteConfig->uniforms.push_back(uniformConfig);
                    }else if(uniformType == "VEC2"){
                        auto uniformConfig = new(std::nothrow)ShaderUniformConfigVec2();
                        uniformConfig->name = uniform[j]["name"].GetString();
                        uniformConfig->value.x = uniform[j]["value"]["x"].GetDouble();
                        uniformConfig->value.y = uniform[j]["value"]["y"].GetDouble();
                        spriteConfig->uniforms.push_back(uniformConfig);
                    }
                }
            }
            
            if(sprites[i].HasMember("timeline")){
                spriteConfig->timeline = sprites[i]["timeline"].GetString();
            }
            
            if(sprites[i].HasMember("blend_func")){
                spriteConfig->setBlendFun(sprites[i]["blend_func"]["src"].GetString(), sprites[i]["blend_func"]["dst"].GetString());
            }
            
            if(sprites[i].HasMember("mask")){
                spriteConfig->mask = sprites[i]["mask"].GetString();
            }
            
            if(sprites[i].HasMember("alpha")){
                spriteConfig->alpha = sprites[i]["alpha"].GetDouble();
            }
            
            if(sprites[i].HasMember("rotation")){
                spriteConfig->rotation = sprites[i]["rotation"].GetDouble();
            }
            
            mConfig.sprites.push_back(spriteConfig);
        }
    }
    
    if(mRoot.HasMember("particles")){
        rapidjson::Value& particles = mRoot["particles"];
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
                
                if(particle.HasMember("scale")){
                    rapidjson::Value& particleScale = particle["scale"];
                    pParticle->scale.x = particleScale["x"].GetDouble();
                    pParticle->scale.y = particleScale["y"].GetDouble();
                    
                }
                
                if(particle.HasMember("radial")){
                    pParticle->radial = particle["radial"].GetBool();
                }
                if(particle.HasMember("frame_tile")){
                    pParticle->frameTile = true;
                    pParticle->tileX = particle["frame_tile"]["x"].GetInt();
                    pParticle->tileY = particle["frame_tile"]["y"].GetInt();
                    pParticle->frameInterval = particle["frame_tile"]["interval"].GetDouble();
                }
                if(particle.HasMember("random_tile")){
                    pParticle->randomTile = true;
                    pParticle->tileX = particle["random_tile"]["x"].GetInt();
                    pParticle->tileY = particle["random_tile"]["y"].GetInt();
                }
                
                
                mConfig.particles.push_back(pParticle);
            }
        }
    }
    
    
    if(mRoot.HasMember("timelines")){
        rapidjson::Value& timelines = mRoot["timelines"];
        if(!timelines.IsNull()){
            for(int i=0; i<timelines.Size(); i++){
                rapidjson::Value& timeline = timelines[i];
                std::string timeId = timeline["id"].GetString();
                mConfig.timelines[timeId] = parseTimeline(timeline);
            }
        }
    }
    
    if(mRoot.HasMember("animations")){
        rapidjson::Value& animations = mRoot["animations"];
        if(!animations.IsNull()){
            for(int i=0; i<animations.Size(); i++){
                rapidjson::Value& animation = animations[i];
                auto animationConfig = new(std::nothrow) AnimationConfig();
                animationConfig->id = animation["id"].GetString();
                animationConfig->frameFile = animation["frame_file"].GetString();
                animationConfig->texture = animation["texture"].GetString();
                animationConfig->frameName = animation["frame_name"].GetString();
                animationConfig->frameFrom = animation["frame_from"].GetInt();
                animationConfig->frameTo = animation["frame_to"].GetInt();
                animationConfig->pos.x = animation["pos"]["x"].GetDouble();
                animationConfig->pos.y = animation["pos"]["y"].GetDouble();
                animationConfig->pos.z = animation["pos"]["z"].GetDouble();
                animationConfig->rotation = animation["rotation"].GetDouble();
                animationConfig->scale.x = animation["scale"]["x"].GetDouble();
                animationConfig->scale.y = animation["scale"]["y"].GetDouble();
                animationConfig->interval = animation["interval"].GetDouble();
                animationConfig->repeat = 0;
                if(animation.HasMember("repeat")){
                    animationConfig->repeat = animation["repeat"].GetInt();
                }
                animationConfig->delay = 0.0f;
                if(animation.HasMember("delay")){
                    animationConfig->delay = animation["delay"].GetDouble();
                }
                if(animation.HasMember("timeline")){
                    animationConfig->timeline = animation["timeline"].GetString();
                }
                mConfig.animations[animationConfig->id] = animationConfig;
            }
        }
    }
    
    if(mRoot.HasMember("masks")){
        rapidjson::Value& masks = mRoot["masks"];
        for(int i=0; i<masks.Size(); i++){
            auto maskConfig = new(std::nothrow) MaskConfig();
            maskConfig->id = masks[i]["id"].GetString();
            maskConfig->stencil = masks[i]["stencil"].GetString();
            maskConfig->offset.x = masks[i]["offset"]["x"].GetDouble();
            maskConfig->offset.y = masks[i]["offset"]["y"].GetDouble();
            maskConfig->alphaThreshold = masks[i]["alpha_threshold"].GetDouble();
            if(masks[i].HasMember("scale")){
                maskConfig->scale.x = masks[i]["scale"]["x"].GetDouble();
                maskConfig->scale.y = masks[i]["scale"]["y"].GetDouble();
            }
            mConfig.masks[maskConfig->id] = maskConfig;
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
    }else if(type == "REPEAT"){
        auto repeatTimeline = new(std::nothrow) TimelineRepeat();
        repeatTimeline->repeat = value["repeat"].GetInt();
        repeatTimeline->child = parseTimeline(value["child"]);
        return repeatTimeline;
    }else if(type == "SPAWN"){
        auto spawnTimeline = new(std::nothrow) TimelineSpawn();
        const rapidjson::Value& children = value["children"];
        for(int i=0; i<children.Size(); i++){
            spawnTimeline->children.push_back(parseTimeline(children[i]));
        }
        return spawnTimeline;
    }else if(type == "SCALE_TO"){
        auto scaleTo = new(std::nothrow) TimelineScaleTo();
        scaleTo->duration = value["duration"].GetDouble();
        scaleTo->x = value["x"].GetDouble();
        scaleTo->y = value["y"].GetDouble();
        return scaleTo;
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
    
    for(std::map<std::string, MaskConfig*>::iterator iter = mConfig.masks.begin(); iter != mConfig.masks.end(); iter++)
    {
        auto stentil = Sprite::create(mConfig.projectPath + iter->second->stencil);
        stentil->setScale(iter->second->scale.x, iter->second->scale.y);
        stentil->setPosition(Vec2::ZERO);
        auto clipNode = ClippingNode::create(stentil);
        
        clipNode->setAlphaThreshold(iter->second->alphaThreshold);
        clipNode->setPosition(iter->second->offset + Vec2(spriteOrigin.width, spriteOrigin.height));
        MainLayer::getInstance()->addMask(iter->second->id, clipNode);
    }
    
    for(std::vector<SpriteConfig*>::iterator iter = mConfig.sprites.begin()
        ; iter != mConfig.sprites.end()
        ; iter++)
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
        if((*iter)->customBlend){
            BlendFunc blendFunc;
            blendFunc.src = (*iter)->blendSrc;
            blendFunc.dst = (*iter)->blendDst;
            shaderSprite->setBlendFunc(blendFunc);
        }
        
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
            }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::FLOAT){
                auto floatUniform = static_cast<ShaderUniformConfigFloat*>(*iterUniform);
                shaderSprite->getGLProgramState()->setUniformFloat(floatUniform->name, floatUniform->value);
            }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::VEC4){
                auto vec4Uniform = static_cast<ShaderUniformConfigVec4*>(*iterUniform);
                shaderSprite->getGLProgramState()->setUniformVec4(vec4Uniform->name, vec4Uniform->value);
            }else if((*iterUniform)->type == SHADER_UNIFORM_TYPE::VEC2){
                auto vec2Uniform = static_cast<ShaderUniformConfigVec2*>(*iterUniform);
                shaderSprite->getGLProgramState()->setUniformVec2(vec2Uniform->name, vec2Uniform->value);
            }
        }
        shaderSprite->setUniformFlag(uniformFlag);
        shaderSprite->setVisible((*iter)->visible);
        shaderSprite->setOpacity(255 * (*iter)->alpha);
        shaderSprite->setRotation((*iter)->rotation);
        if((*iter)->timeline.length() > 0){
            shaderSprite->runAction(mConfig.timelines[(*iter)->timeline]->getAction());
        }
        
        if((*iter)->mask.length() > 0){
            MainLayer::getInstance()->addSprite((*iter)->id, shaderSprite, (*iter)->mask);
        }else{
            MainLayer::getInstance()->addSprite((*iter)->id, shaderSprite, zOrder);
        }
        
    }
    
    for(std::vector<ParticleConfig*>::iterator iter = mConfig.particles.begin();
        iter != mConfig.particles.end();
        iter++){
        auto particle = ParticleSystemExt::create(mConfig.projectPath + (*iter)->file);
        particle->setPosition((*iter)->position.x + spriteOrigin.width
                              , (*iter)->position.y+spriteOrigin.height);
        particle->MainLayer::setScale((*iter)->scale.x, (*iter)->scale.y);
        particle->setRadial((*iter)->radial);
        if((*iter)->frameTile){
            particle->setFrameTile((*iter)->tileX, (*iter)->tileY, (*iter)->frameInterval);
        }
        if((*iter)->timeline.length() > 0){
            particle->runAction(mConfig.timelines[(*iter)->timeline]->getAction());
        }
        if((*iter)->randomTile){
            particle->setRandomFrame((*iter)->tileX, (*iter)->tileY);
        }
        MainLayer::getInstance()->addParticleSystem((*iter)->id, particle, (*iter)->position.z);
    }

    for(std::map<std::string, AnimationConfig*>::iterator iter = mConfig.animations.begin();
        iter != mConfig.animations.end();
        iter++)
    {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(mConfig.projectPath + iter->second->frameFile
                                                                 , mConfig.projectPath + iter->second->texture);
        int frameFrom = iter->second->frameFrom;
        int frameTo = iter->second->frameTo;
        cocos2d::Vector<SpriteFrame*> frames;
        for(int i=frameFrom; i<=frameTo; i++){
            std::string frameName = Utils::stringFormat(iter->second->frameName.c_str(), 256, i);
            auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
            frames.pushBack(spriteFrame);
        }
        auto animations = Animation::createWithSpriteFrames(frames, iter->second->interval*2);
        auto animator = Animate::create(animations);
        auto sprite = ShaderSprite::create();
        sprite->setPosition(iter->second->pos.x + spriteOrigin.width, iter->second->pos.y + spriteOrigin.height);
        sprite->setScale(iter->second->scale.x, iter->second->scale.y);
        sprite->setRotation(iter->second->rotation);
        sprite->setBlendFunc(BlendFunc::ADDITIVE);
        sprite->runAction(Sequence::create(DelayTime::create(iter->second->delay), Repeat::create(animator, iter->second->repeat), NULL));
        sprite->runAction(animator);
        if(iter->second->timeline.length() > 0){
            sprite->runAction(mConfig.timelines[iter->second->timeline]->getAction());
        }
        
        MainLayer::getInstance()->addSprite(iter->first, sprite, iter->second->pos.z);
    }
}

bool Project::saveProject()
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    mRoot.Accept(writer);
    mConfig.projectString = buffer.GetString();
    return FileUtils::getInstance()->writeStringToFile(mConfig.projectString, mConfig.projectFilePath);
}

ProjectConfig* Project::getConfig()
{
    return &mConfig;
}

NS_EE_END