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
#include "TimelineManager.hpp"
#include "PlatformAdapter.h"

NS_EE_BEGIN

Project::Project()
{
}

Project::~Project()
{
    mConfig.version = "1.0.0.1";
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
                std::string vShaderFile = mConfig.projectPath + sprites[i]["v_shader"].GetString();
                if(!FileUtils::getInstance()->isFileExist(vShaderFile)){
                    vShaderFile = std::string("shader/") + sprites[i]["v_shader"].GetString();
                    std::string message = sprites[i]["v_shader"].GetString();
                    message += "not exit";
                    CCASSERT(FileUtils::getInstance()->isFileExist(vShaderFile), message.c_str());
                }
                spriteConfig->vShader = vShaderFile;
                
            }else{
                spriteConfig->vShader = "shader/shader_default_vert.glsl";
            }
            
            if(sprites[i].HasMember("f_shader")){
                std::string fShader = mConfig.projectPath + sprites[i]["f_shader"].GetString();
                if(!FileUtils::getInstance()->isFileExist(fShader)){
                    fShader = std::string("shader/")+sprites[i]["f_shader"].GetString();
                    std::string message = sprites[i]["f_shader"].GetString();
                    message += "not exit";
                    CCASSERT(FileUtils::getInstance()->isFileExist(fShader), message.c_str());
                }
                spriteConfig->fShader = fShader;
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
            
            if(sprites[i].HasMember("anchor")){
                rapidjson::Value &scale = sprites[i]["anchor"];
                spriteConfig->anchor.x = scale["x"].GetDouble();
                spriteConfig->anchor.y = scale["y"].GetDouble();
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
                if(animation.HasMember("visible")){
                    animationConfig->visible = animation["visible"].GetBool();
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
    return true;
}

Timeline* Project::parseTimeline(const rapidjson::Value &value)
{
    if(value.HasMember("file")){
        return TimelineManager::getInstance()->getTimelineFromFile(mConfig.projectPath + value["file"].GetString());
    }else{
        return TimelineManager::getInstance()->getTimelineFromJson(value);
    }
}

bool Project::saveProject()
{
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    mRoot.Accept(writer);
    mConfig.projectString = buffer.GetString();
    std::string savePath = PlatformAdapter::getSaveFilePath("json");
    if(savePath.length() > 0){
        std::string formatString = Utils::jsonFormat(mConfig.projectString.c_str());
        return FileUtils::getInstance()->writeStringToFile(formatString, savePath);
    }
    return false;
}

ProjectConfig* Project::getConfig()
{
    return &mConfig;
}

NS_EE_END
