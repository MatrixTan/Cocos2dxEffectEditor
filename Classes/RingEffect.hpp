//
//  RingEffect.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/28.
//
//

#ifndef RingEffect_hpp
#define RingEffect_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include <cocos2d.h>
#include "ShaderSprite.hpp"

NS_EE_BEGIN

USING_NS_CC;

class RingEffect : public Node
{
public:
    static RingEffect* create(float speed, float duration);
    
    RingEffect();
    
    bool init(float speed, float duration);
    virtual void update(float dt) override;
    
private:
    ParticleSystemQuad *mParticle;
    float mStartRange;
    float mEndRange;
    float mSpeed;
    float mParticleCountFactor;
    float mStartTime;
    float mDuration;
    ShaderSprite *mRing;
};

NS_EE_END


#endif /* RingEffect_hpp */
