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
mLinkParticle(nullptr)
{
}

LinkEffect::~LinkEffect()
{
}

LinkEffect* LinkEffect::create(cocos2d::Node *from, cocos2d::Node *to)
{
    auto link = new(std::nothrow)LinkEffect();
    if(link != nullptr && link->init(from, to)){
        link->autorelease();
    }else{
        CC_SAFE_DELETE(link);
    }
    return link;
}

bool LinkEffect::init(cocos2d::Node *from, cocos2d::Node *to)
{
    mFrom = from;
    CC_SAFE_RETAIN(mFrom);
    mTo = to;
    CC_SAFE_RETAIN(mTo);
    mLinkParticle = ParticleSystemQuad::create("res/link_light.plist");
    mLinkParticle->setPosition(Vec2::ZERO);
    addChild(mLinkParticle);
    scheduleUpdate();
    
    auto test = Sprite::create("projects/item_20.png");
    addChild(test);
    test->setScale(0.05);
    
    return Node::init();
}


void LinkEffect::update(float dt)
{
    if(mFrom != nullptr && mTo != nullptr){
        Vec2 fromPos = mFrom->getPosition();
        Vec2 toPos = mTo->getPosition();
        setPosition((fromPos + toPos) * 0.5f);
        Vec2 offset = toPos - fromPos;
        float angle = CC_RADIANS_TO_DEGREES(offset.getAngle());
        mLinkParticle->setRotation(-angle);
        mLinkParticle->setPosVar(Vec2(offset.length() * 0.5, 0.0f));
        mLinkParticle->setLife(0.16f);
        mLinkParticle->setEmissionRate(1000.0f);
    }
    Node::update(dt);
}


NS_EE_END