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
    
    for(int i=0; i<LINK_EFFECT_COUNT; i++){
        mLinkParticles[i] = ParticleSystemQuad::create(__String::createWithFormat("res/link_light%d.plist", i)->getCString());
        mLinkParticles[i]->setLife(0.1f);
        addChild(mLinkParticles[i]);
    }
    //mLinkParticles[1]->setLife(0.1f);
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
            float length = offset.length();
            int particalNumber = floorf(length * 0.5);
            for(int i=0; i<LINK_EFFECT_COUNT; i++){
                mLinkParticles[i]->setRotation(-angle);
                mLinkParticles[i]->setPosVar(Vec2(length * 0.5, 0.0f));
            }
            if(abs(particalNumber - mLinkParticles[0]->getTotalParticles()) > 50)
            {
                mLinkParticles[0]->setTotalParticles(particalNumber);
            }

        }
        
    }
    Node::update(dt);
}


NS_EE_END