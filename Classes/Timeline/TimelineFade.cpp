//
//  TimelineFade.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/6.
//
//

#include "TimelineFade.hpp"

NS_EE_BEGIN

TimelineFade::~TimelineFade()
{
}

FiniteTimeAction* TimelineFade::getAction()
{
    return FadeTo::create(duration, 255 * to);
}

NS_EE_END
