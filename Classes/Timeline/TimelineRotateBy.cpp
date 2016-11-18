//
//  TimelineRotateBy.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#include "TimelineRotateBy.hpp"

NS_EE_BEGIN

FiniteTimeAction* TimelineRotateBy::getAction()
{
    return RotateBy::create(duration, angle);
}

NS_EE_END