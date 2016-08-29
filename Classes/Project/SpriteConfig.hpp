//
//  SpriteConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#ifndef SpriteConfig_hpp
#define SpriteConfig_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

using namespace cocos2d;

enum class SPRITE_SOURCE_TYPE
{
    NONE,
    FILE,
    ATLAS
};

//enum class

class ShaderUniformConfig;

class SpriteConfig
{
public:
    SpriteConfig();
    ~SpriteConfig();
    
    std::string id;
    std::string texture;
    SPRITE_SOURCE_TYPE sourceType;
    Vec3 position;
    Vec2 scale;
    float rotation;
    std::string vShader;
    std::string fShader;
    std::vector<ShaderUniformConfig*> uniforms;
    bool visible;
    std::string timeline;
    float alpha;
    
    GLenum blendSrc;
    GLenum blendDst;
    bool customBlend;
    
    std::string mask;
    
    void setBlendFun(const std::string& src, const std::string& dst);
    
    static SPRITE_SOURCE_TYPE getSpriteSouceType(const std::string& type);
    
private:
    GLenum getBlendFunc(const std::string& name);
};

NS_EE_END

#endif /* SpriteConfig_hpp */