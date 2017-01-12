//
//  TimelineEase.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/5.
//
//

#include "TimelineEase.hpp"

NS_EE_BEGIN

TimelineEase::~TimelineEase()
{
    CC_SAFE_DELETE(child);
}

FiniteTimeAction* TimelineEase::getAction()
{
    if(child != nullptr){
        ActionInterval* interval = static_cast<ActionInterval*>(child->getAction());
        if(interval){
             return EaseIn::create(interval, rate);
        }
    }
    return nullptr;
}

NS_EE_END
