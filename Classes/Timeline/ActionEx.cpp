//
//  ActionEx.cpp
//  EffectEditor
//
//  Created by Matrix on 16/10/18.
//
//

#include "ActionEx.hpp"

NS_EE_BEGIN

BezierRotateTo* BezierRotateTo::create(float duration, const ccBezierConfig& config)
{
    BezierRotateTo* pRet = new BezierRotateTo();
    if (pRet != nullptr && pRet->initWithDuration(duration, config))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return nullptr;
}

void BezierRotateTo::startWithTarget(Node* pTarget)
{
    BezierTo::startWithTarget(pTarget);
    mLastPos = pTarget->getPosition();
    float angle = (_toConfig.controlPoint_1 - mLastPos).getAngle();
    pTarget->setRotation(-1 * angle / M_PI * 180.0f + 90.0f);

}

void BezierRotateTo::update(float time)
{
    BezierTo::update(time);
    float angle = (_target->getPosition() - mLastPos).getAngle();
    _target->setRotation(-1 * angle / M_PI * 180.0f + 90.0f);
    mLastPos = _target->getPosition();
}

BezierRotateTo* BezierRotateTo::clone() const
{
    return BezierRotateTo::create(_duration, _toConfig);
}


NS_EE_END