//
//  TimelineScaleTo.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#include "TimelineScaleTo.hpp"

NS_EE_BEGIN

//========TimelineScaleTo==========
FiniteTimeAction* TimelineScaleTo::getAction()
{
    return ScaleTo::create(duration, x, y);
}


NS_EE_END