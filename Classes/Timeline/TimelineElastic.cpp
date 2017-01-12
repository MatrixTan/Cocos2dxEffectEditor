//
//  TimelineElastic.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/5.
//
//

#include "TimelineElastic.hpp"

NS_EE_BEGIN

TimelineElastic::~TimelineElastic()
{
    CC_SAFE_DELETE(child);
}

FiniteTimeAction* TimelineElastic::getAction()
{
    return nullptr;
}


TimelineElasticIn::~TimelineElasticIn()
{

}

FiniteTimeAction* TimelineElasticIn::getAction()
{
    if(child != nullptr){
        ActionInterval* interval = static_cast<ActionInterval*>(child->getAction());
        if(interval){
            return EaseElasticIn::create(interval, p);
        }
    }
    return nullptr;
}

TimelineElasticOut::~TimelineElasticOut()
{

}

FiniteTimeAction* TimelineElasticOut::getAction()
{
    if(child != nullptr){
        ActionInterval* interval = static_cast<ActionInterval*>(child->getAction());
        if(interval){
            return EaseElasticOut::create(interval, p);
        }
    }
    return nullptr;
}

TimelineElasticInOut::~TimelineElasticInOut()
{

}

FiniteTimeAction* TimelineElasticInOut::getAction()
{
    if(child != nullptr){
        ActionInterval* interval = static_cast<ActionInterval*>(child->getAction());
        if(interval){
            return EaseElasticInOut::create(interval, p);
        }
    }
    return nullptr;
}

NS_EE_END
