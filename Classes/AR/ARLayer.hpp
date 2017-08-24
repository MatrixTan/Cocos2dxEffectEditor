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
#include "ARCamera.hpp"

NS_EE_BEGIN

using namespace cocos2d;

class ARLayer: public Layer{
public:
    virtual bool init() override;
    virtual void update(float dt) override;
private:
    CustomCommand mRenderCommand;
    V3F_C4B_T2F_Quad mQuad;
    Texture2D* test = nullptr;
    ShaderSprite* mCameraSprite = nullptr;
    ARCamera* mARCamera = nullptr;
};

NS_EE_END

#endif /* ARLayer_hpp */
