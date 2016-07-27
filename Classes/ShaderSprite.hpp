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
#include <string>

NS_EE_BEGIN

using namespace cocos2d;

enum class SHADER_UNIFORM_FLAG
{
    TIME = 1,
    LIGHT_DIR = 1 << 1
};

class ShaderSprite : public cocos2d::Sprite{
public:
    ShaderSprite();
    ~ShaderSprite();
    virtual bool init() override;
    virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4 &transform, uint32_t flags) override;
    virtual void update(float dt) override;
    
    CREATE_FUNC(ShaderSprite);
    
    bool initShader(const std::string& vfile, const std::string& ffile);
    void setUniformFlag(uint32_t flag);
    void resetTimeUniform();
    
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
