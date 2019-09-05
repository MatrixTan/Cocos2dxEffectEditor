//
//  ProjectConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#include "ProjectConfig.hpp"
#include "SpriteConfig.hpp"
#include "ShaderUniformConfig.hpp"
#include "ParticleConfig.hpp"
#include "AnimationConfig.hpp"
#include "EngineEx/ParticleSystemExt.hpp"
#include "MaskConfig.hpp"
#include "Timeline/TimelineManager.hpp"
#include "Common/Utils.hpp"

NS_EE_BEGIN

ProjectConfig::ProjectConfig()
{
    version = "1.0.0.1";
}

ProjectConfig::~ProjectConfig()
{
    for(auto sprite : sprites){
        delete sprite.second;
    }
    sprites.clear();
    
    for(auto particle : particles){
        delete particle.second;
    }
    particles.clear();
    
    for(auto timeline : timelines){
        delete timeline.second;
    }
    timelines.clear();
    
    for(auto animation : animations){
        delete animation.second;
    }
    animations.clear();
    
    for(auto mask : masks){
        delete mask.second;
    }
    masks.clear();
}

bool ProjectConfig::loadFile(const std::string &file)
{
    projectPath = file;
    std::string projectString = FileUtils::getInstance()->getStringFromFile(file);
    projectFilePath = FileUtils::getInstance()->fullPathForFilename(projectPath);
    projectPath = projectFilePath.substr(0, projectFilePath.find_last_of('/')) + "/";
    
    rapidjson::Document documentRoot;
    documentRoot.Parse<0>(projectString.c_str());
    if(documentRoot.HasParseError()){
        CCASSERT(false, "project parse error");
        return false;
    }
    
    
    version = documentRoot["version"].GetString();
    
    if(documentRoot.HasMember("background")){
        background.file = documentRoot["background"]["file"].GetString();
        background.scale.x = documentRoot["background"]["scale"]["x"].GetDouble();
        background.scale.y = documentRoot["background"]["scale"]["y"].GetDouble();
    }
    
    if(documentRoot.HasMember("atlas")){
        rapidjson::Value &atlasArray = documentRoot["atlas"];
        for(int i=0; i<atlasArray.Size(); i++){
            atlas.push_back(atlasArray[i].GetString());
        }
    }
    
    if(documentRoot.HasMember("sprites")){
        rapidjson::Value &spritesJson = documentRoot["sprites"];
        for(int i=0; i<spritesJson.Size(); i++){
            auto spriteConfig = new(std::nothrow)SpriteConfig();
            spriteConfig->id = spritesJson[i]["id"].GetString();
            spriteConfig->texture = spritesJson[i]["texture"].GetString();
            spriteConfig->visible = spritesJson[i]["visible"].GetBool();
            std::string sourceType = spritesJson[i]["source_type"].GetString();
            spriteConfig->sourceType = SpriteConfig::getSpriteSouceType(sourceType);
            
            if(spritesJson[i].HasMember("v_shader")){
                std::string vShaderFile = projectPath + spritesJson[i]["v_shader"].GetString();
                if(!FileUtils::getInstance()->isFileExist(vShaderFile)){
                    vShaderFile = std::string("shader/") + spritesJson[i]["v_shader"].GetString();
                    std::string message = spritesJson[i]["v_shader"].GetString();
                    message += "not exit";
                    CCASSERT(FileUtils::getInstance()->isFileExist(vShaderFile), message.c_str());
                }
                spriteConfig->vShader = vShaderFile;
                
            }else{
                spriteConfig->vShader = "shader/shader_default_vert.glsl";
            }
            
            if(spritesJson[i].HasMember("f_shader")){
                std::string fShader = projectPath + spritesJson[i]["f_shader"].GetString();
                if(!FileUtils::getInstance()->isFileExist(fShader)){
                    fShader = std::string("shader/")+spritesJson[i]["f_shader"].GetString();
                    std::string message = spritesJson[i]["f_shader"].GetString();
                    message += "not exit";
                    CCASSERT(FileUtils::getInstance()->isFileExist(fShader), message.c_str());
                }
                spriteConfig->fShader = fShader;
            }else{
                spriteConfig->fShader = "shader/shader_default_frag.glsl";
            }
            
            rapidjson::Value &position = spritesJson[i]["pos"];
            
            spriteConfig->position.x = position["x"].GetDouble();
            spriteConfig->position.y = position["y"].GetDouble();
            spriteConfig->position.z = position["z"].GetDouble();
            
            if(spritesJson[i].HasMember("scale")){
                rapidjson::Value &scale = spritesJson[i]["scale"];
                spriteConfig->scale.x = scale["x"].GetDouble();
                spriteConfig->scale.y = scale["y"].GetDouble();
            }
            
            if(spritesJson[i].HasMember("anchor")){
                rapidjson::Value &scale = spritesJson[i]["anchor"];
                spriteConfig->anchor.x = scale["x"].GetDouble();
                spriteConfig->anchor.y = scale["y"].GetDouble();
            }
            
            if(spritesJson[i].HasMember("uniform")){
                rapidjson::Value &uniform = spritesJson[i]["uniform"];
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
            
            if(spritesJson[i].HasMember("timeline")){
                spriteConfig->timeline = spritesJson[i]["timeline"].GetString();
            }
            
            if(spritesJson[i].HasMember("blend_func")){
                spriteConfig->setBlendFun(spritesJson[i]["blend_func"]["src"].GetString(), spritesJson[i]["blend_func"]["dst"].GetString());
            }
            
            if(spritesJson[i].HasMember("mask")){
                spriteConfig->mask = spritesJson[i]["mask"].GetString();
            }
            
            if(spritesJson[i].HasMember("alpha")){
                spriteConfig->alpha = spritesJson[i]["alpha"].GetDouble();
            }
            
            if(spritesJson[i].HasMember("rotation")){
                spriteConfig->rotation = spritesJson[i]["rotation"].GetDouble();
            }
            
            sprites[spriteConfig->id] = spriteConfig;
        }
    }
    
    if(documentRoot.HasMember("particles")){
        rapidjson::Value& particlesJson = documentRoot["particles"];
        if(!particlesJson.IsNull()){
            for(int i=0;i<particlesJson.Size();i++){
                rapidjson::Value& particle = particlesJson[i];
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
                particles[pParticle->id] = pParticle;
            }
        }
    }
    
    
    if(documentRoot.HasMember("timelines")){
        rapidjson::Value& timelinesJson = documentRoot["timelines"];
        if(!timelinesJson.IsNull()){
            for(int i=0; i<timelinesJson.Size(); i++){
                rapidjson::Value& timeline = timelinesJson[i];
                std::string timeId = timeline["id"].GetString();
                timelines[timeId] = parseTimeline(timeline);
            }
        }
    }
    
    if(documentRoot.HasMember("animations")){
        rapidjson::Value& animationsJson = documentRoot["animations"];
        if(!animationsJson.IsNull()){
            for(int i=0; i<animationsJson.Size(); i++){
                rapidjson::Value& animation = animationsJson[i];
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
                animations[animationConfig->id] = animationConfig;
            }
        }
    }
    
    if(documentRoot.HasMember("masks")){
        rapidjson::Value& masksJson = documentRoot["masks"];
        for(int i=0; i<masksJson.Size(); i++){
            auto maskConfig = new(std::nothrow) MaskConfig();
            maskConfig->id = masksJson[i]["id"].GetString();
            maskConfig->stencil = masksJson[i]["stencil"].GetString();
            maskConfig->offset.x = masksJson[i]["offset"]["x"].GetDouble();
            maskConfig->offset.y = masksJson[i]["offset"]["y"].GetDouble();
            maskConfig->alphaThreshold = masksJson[i]["alpha_threshold"].GetDouble();
            if(masksJson[i].HasMember("scale")){
                maskConfig->scale.x = masksJson[i]["scale"]["x"].GetDouble();
                maskConfig->scale.y = masksJson[i]["scale"]["y"].GetDouble();
            }
            masks[maskConfig->id] = maskConfig;
        }
    }
    return true;
}

Timeline* ProjectConfig::parseTimeline(const rapidjson::Value &value)
{
    if(value.HasMember("file")){
        return TimelineManager::getInstance()->getTimelineFromFile(projectPath + value["file"].GetString());
    }else{
        return TimelineManager::getInstance()->getTimelineFromJson(value);
    }
}

std::string ProjectConfig::getJsonString() const
{
    rapidjson::Document rootJson;
    rootJson.SetObject();
    
    rapidjson::Document::AllocatorType &locator = rootJson.GetAllocator();
    rapidjson::Value versionJson(rapidjson::kStringType);
    versionJson.SetString(version.c_str(), (unsigned int)version.size());
    rootJson.AddMember("version", versionJson, locator);
    //rootJson["version"].SetString(version.c_str());
    
    if(background.file.length() > 0){

    }
    
    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    rootJson.Accept(writer);
 
    std::string formatString = Utils::jsonFormat(buffer.GetString());
    return formatString;
}


NS_EE_END
