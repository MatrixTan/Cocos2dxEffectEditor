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
    auto version = root["version"].GetString();
    
    auto background = root["background"].GetString();
    MainLayer::getInstance()->setBackground(directoryPath + background);
    
    auto spriteOrigin = Director::getInstance()->getWinSize() * 0.5f;
    
    rapidjson::Value &sprites = root["sprites"];
    for(int i=0; i<sprites.Size(); i++)
    {
        auto shaderSprite = ShaderSprite::create();
        std::string texture = sprites[i]["texture"].GetString();
        shaderSprite->initWithFile(directoryPath + texture);
        
        std::string vShader;
        if(sprites[i].HasMember("v_shader"))
        {
            vShader = sprites[i]["v_shader"].GetString();
        }
        std::string fShader;
        if(sprites[i].HasMember("f_shader"))
        {
            fShader = sprites[i]["f_shader"].GetString();
        }
        shaderSprite->initShader(vShader.size()>0?directoryPath+vShader:"", fShader.size()>0?directoryPath+fShader:"");
        
        rapidjson::Value &position = sprites[i]["pos"];
        Vec2 pos;
        pos.x = position["x"].GetDouble() + spriteOrigin.width;
        pos.y = position["y"].GetDouble() + spriteOrigin.height;
        int zOrder = position["z"].GetInt();
        shaderSprite->setPosition(pos);
        shaderSprite->setLocalZOrder(zOrder);
        
        uint32_t uniformFlag = 0;
        if(sprites[i].HasMember("uniform"))
        {
            rapidjson::Value &uniform = sprites[i]["uniform"];
            for(int j=0; j<uniform.Size(); j++)
            {
                std::string x = uniform[j]["type"].GetString();
                if(0 == strcmp(uniform[j]["type"].GetString() , "TEXTURE"))
                {
                    std::string textureFile = uniform[j]["file"].GetString();
                    std::string name = uniform[j]["name"].GetString();
                    auto texture = TextureCache::getInstance()->addImage(directoryPath + textureFile);
                    shaderSprite->getGLProgramState()->setUniformTexture(name, texture);
                }
                else if(0 == strcmp(uniform[j]["type"].GetString() ,"TIME"))
                {
                    uniformFlag |= (uint32_t) SHADER_UNIFORM_FLAG::TIME;
                }
            }

        }
        shaderSprite->setUniformFlag(uniformFlag);
        
        MainLayer::getInstance()->addSprite(shaderSprite);
    }
}

NS_EE_END