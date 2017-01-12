//
//  TimelineDelay.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/6.
//
//

#include "TimelineDelay.hpp"

NS_EE_BEGIN

FiniteTimeAction* TimelineDelay::getAction()
{
    return DelayTime::create(delay);
}


NS_EE_END
