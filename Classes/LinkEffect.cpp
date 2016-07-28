//
//  LinkEffect.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/27.
//
//

#include "LinkEffect.hpp"

NS_EE_BEGIN

LinkEffect::LinkEffect()
:mFrom(nullptr),
mTo(nullptr),
mLinkParticle(nullptr),
mState(LINK_EFFECT_STATE::NONE),
mLinkSpeed(0.0f),
mLinkTime(0.0f)
{
}

LinkEffect::~LinkEffect()
{
    CC_SAFE_RELEASE(mFrom);
    CC_SAFE_RELEASE(mTo);
}

LinkEffect* LinkEffect::create(cocos2d::Node *from, cocos2d::Node *to, float speed)
{
    auto link = new(std::nothrow)LinkEffect();
    if(link != nullptr && link->init(from, to, speed)){
        link->autorelease();
    }else{
        CC_SAFE_DELETE(link);
    }
    return link;
}

bool LinkEffect::init(cocos2d::Node *from, cocos2d::Node *to, float speed)
{
    mFrom = from;
    CC_SAFE_RETAIN(mFrom);
    mTo = to;
    CC_SAFE_RETAIN(mTo);
    mLinkSpeed = speed;
    if(mLinkSpeed <= 0.0){
        mState = LINK_EFFECT_STATE::STABLE;
    }else{
        mState = LINK_EFFECT_STATE::LINK_TO;
    }
    
    mLinkParticle = ParticleSystemQuad::create("res/link_light.plist");
    mLinkParticle->setPosition(Vec2::ZERO);
    mLinkParticle->setLife(0.1f);
    addChild(mLinkParticle);
    scheduleUpdate();
    
    return Node::init();
}


void LinkEffect::update(float dt)
{
    if(mFrom != nullptr && mTo != nullptr){
        if(mState == LINK_EFFECT_STATE::LINK_TO || mState == LINK_EFFECT_STATE::STABLE){
            Vec2 fromPos = mFrom->getPosition();
            Vec2 toPos = mTo->getPosition();
            Vec2 offset = toPos - fromPos;
            
            if(mState == LINK_EFFECT_STATE::LINK_TO){
                mLinkTime += dt;
                float moveDistance = mLinkSpeed * mLinkTime;
                if(moveDistance < offset.length()){
                    offset.normalize();
                    offset = offset * moveDistance;
                    toPos = fromPos +  offset;
                    
                }else{
                    mState = LINK_EFFECT_STATE::STABLE;
                }
            }
            
            setPosition((fromPos + toPos) * 0.5f);
            float angle = CC_RADIANS_TO_DEGREES(offset.getAngle());
            mLinkParticle->setRotation(-angle);
            float length = offset.length();
            mLinkParticle->setPosVar(Vec2(length * 0.5, 0.0f));
            int particalNumber = floorf(length * 0.5);
            if(abs(particalNumber - mLinkParticle->getTotalParticles()) > 50)
            {
                mLinkParticle->setTotalParticles(particalNumber);
            }
        }
        
    }
    Node::update(dt);
}


NS_EE_END