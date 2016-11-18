//
//  TimelineRepeat.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#include "TimelineRepeat.hpp"

NS_EE_BEGIN

//==========TimelineRepeat==============
TimelineRepeat::~TimelineRepeat()
{
    CC_SAFE_DELETE(child);
}

FiniteTimeAction* TimelineRepeat::getAction()
{
    return Repeat::create(child->getAction(), repeat);
}

NS_EE_END