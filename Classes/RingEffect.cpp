//
//  RingEffect.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/28.
//
//

#include "RingEffect.hpp"

NS_EE_BEGIN

RingEffect* RingEffect::create(float speed, float duration)
{
    auto effect = new(std::nothrow) RingEffect();
    if(effect != nullptr && effect->init(speed, duration)){
        effect->autorelease();
    }
    return effect;
}

RingEffect::RingEffect()
:mParticle(nullptr)
{
}

bool RingEffect::init(float speed,float duration)
{
    mSpeed = speed;
    mDuration = duration;
    mParticle = ParticleSystemQuad::create("res/ring_effect.plist");
    addChild(mParticle);
    mParticle->setPosition(Vec2::ZERO);
    mStartRange = mParticle->getStartRadius();
    mEndRange = mParticle->getEndRadius();
    mParticleCountFactor = mParticle->getTotalParticles() / mStartRange;
    
    mRing = ShaderSprite::create();
    mRing->initWithFile("res/ring.png");
    mRing->initShader("shader/shader_ring_vert.glsl", "shader/shader_ring_frag.glsl");
    mRing->getGLProgramState()->setUniformFloat("u_threshold_ring", 0.4);
    mRing->setPosition(0.0f, 0.0f);
    addChild(mRing);
    
    scheduleUpdate();
    return Node::init();
}

void RingEffect::update(float dt)
{
    mStartTime += dt;
    float startRange = mStartRange + mStartTime * mSpeed;
    mParticle->setStartRadius(startRange);
    mParticle->setEndRadius(mEndRange + mStartTime * mSpeed);
    mRing->setScale(startRange/mStartRange);
    mRing->getGLProgramState()->setUniformFloat("u_scale", startRange/mStartRange);
    if(startRange * mParticleCountFactor - mParticle->getTotalParticles() > 800){
        mParticle->setTotalParticles(startRange * mParticleCountFactor);
    }
    
    if(mStartTime > mDuration){
        removeFromParentAndCleanup(true);
    }
    
    return Node::update(dt);
}

NS_EE_END