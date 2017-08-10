//
//  ARLayer.hpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/9.
//

#ifndef ARLayer_hpp
#define ARLayer_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "ShaderSprite.hpp"

NS_EE_BEGIN

using namespace cocos2d;

class ARLayer: public Layer{
public:
    virtual bool init() override;
    virtual void update(float dt) override;
    void onDraw(const Mat4 &transfrom, uint32_t flags);
private:
    CustomCommand mRenderCommand;
    V3F_C4B_T2F_Quad mQuad;
    Texture2D* test;
    ShaderSprite* mCameraSprite;
};

NS_EE_END

#endif /* ARLayer_hpp */
