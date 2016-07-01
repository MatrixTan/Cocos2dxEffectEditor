//
//  ShaderSprite.hpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#ifndef ShaderSprite_hpp
#define ShaderSprite_hpp


#include "GlobalDefine.hpp"
#include <stdio.h>

NS_EE_BEGIN

using namespace cocos2d;

enum class ITEM_SHADER_UNIFORM
{
    ITEM_SHADER_UNIFORM_TIME = 1,
    ITEM_SHADER_UNIFORM_LIGHT_DIR = 1 << 1
};

class ShaderSprite : public cocos2d::Sprite{
public:
    ShaderSprite();
    ~ShaderSprite();
    virtual bool init() override;
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    virtual void update(float dt) override;
    
    CREATE_FUNC(ShaderSprite);
    
private:
    
    cocos2d::GLProgram* getProgram(const std::string& vfile, const std::string& ffile);
    void setStateCallback(float t);
    
    cocos2d::GLProgramState *m_programState;
    float m_startTime;
    cocos2d::Vec3 m_lightPos;
    uint32_t m_uniformFlag;

};


NS_EE_END

#endif /* ShaderSprite_hpp */
