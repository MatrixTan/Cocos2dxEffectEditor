//
//  TimelineConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/1.
//
//

#include "TimelineConfig.hpp"

NS_EE_BEGIN

FiniteTimeAction* TimelineMoveBy::getAction()
{
    return MoveBy::create(duration, Vec2(x, y));
}


//========TimelineSequence==========
TimelineSequence::~TimelineSequence()
{
    for(std::vector<Timeline*>::iterator iter = children.begin(); iter != children.end(); iter++)
    {
        CC_SAFE_DELETE(*iter);
    }
}

FiniteTimeAction* TimelineSequence::getAction()
{
    Vector<FiniteTimeAction*> actions;
    for(std::vector<Timeline*>::iterator iter = children.begin(); iter != children.end(); iter++)
    {
        actions.pushBack((*iter)->getAction());
    }
    return Sequence::create(actions);
}

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