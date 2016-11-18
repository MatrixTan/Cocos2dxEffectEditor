//
//  TimelineSequence.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#include "TimelineSequence.hpp"


NS_EE_BEGIN
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
NS_EE_END