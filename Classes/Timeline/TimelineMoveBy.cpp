//
//  TimelineMoveBy.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#include "TimelineMoveBy.hpp"

NS_EE_BEGIN

//========TimelineMoveBy===========
FiniteTimeAction* TimelineMoveBy::getAction()
{
    return MoveBy::create(duration, Vec2(x, y));
}

NS_EE_END