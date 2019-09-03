//
//  ThunderLinkEffect.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/9.
//
//

#include "Effect/ThunderLinkEffect.hpp"
#include "EngineEx/ShaderSprite.hpp"

NS_EE_BEGIN

ThunderLinkEffect::ThunderLinkEffect()
:mFrom(nullptr),
mTo(nullptr),
mLinkSpeed(0.0f)
{
}

ThunderLinkEffect::~ThunderLinkEffect()
{
    CC_SAFE_RELEASE(mFrom);
    CC_SAFE_RELEASE(mTo);
}

ThunderLinkEffect* ThunderLinkEffect::create(cocos2d::Node *from, cocos2d::Node *to, float speed)
{
    auto link = new(std::nothrow)ThunderLinkEffect();
    if(link != nullptr && link->init(from, to, speed)){
        link->autorelease();
    }else{
        CC_SAFE_DELETE(link);
    }
    return link;
}

bool ThunderLinkEffect::init(cocos2d::Node *from, cocos2d::Node *to, float speed)
{
    mFrom = from;
    CC_SAFE_RETAIN(mFrom);
    mTo = to;
    CC_SAFE_RETAIN(mTo);
    mLinkSpeed = speed;
    
    setPosition(mFrom->getPosition());
    
    mSprite = ShaderSprite::create();
    mSprite->initWithFile("projects/lightning.png");
    mSprite->initShader("shader/shader_default_vert.glsl", "projects/shader_uv_transfer.glsl");
    addChild(mSprite);
    mSprite->getGLProgramState()->setUniformFloat("u_speed", mLinkSpeed);
    mSprite->getGLProgramState()->setUniformVec4("u_color", Vec4(1.0, 1.0, 1.0, 1.0));
    mSprite->setUniformFlag((int)SHADER_UNIFORM_FLAG::TIME);
    mSprite->setAnchorPoint(Vec2(0.5f, 0.0f));
    mSprite->setPosition(Vec2::ZERO);
    updateThunder();
    scheduleUpdate();
    return Node::init();
}


void ThunderLinkEffect::update(float dt)
{
    updateThunder();
    Node::update(dt);
}

void ThunderLinkEffect::updateThunder()
{
    float height = mSprite->getContentSize().height;
    Vec2 offset = mTo->getPosition() - mFrom->getPosition();
    float scale = offset.length() / height;
    mSprite->setScale(1.0f, scale);
    mSprite->getGLProgramState()->setUniformFloat("u_window_width", scale);
    mSprite->setRotation(CC_RADIANS_TO_DEGREES(offset.getAngle(Vec2(0.0, 1.0))));
}

NS_EE_END