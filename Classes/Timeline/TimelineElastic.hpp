//
//  TimelineElastic.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/5.
//
//

#ifndef TimelineElastic_hpp
#define TimelineElastic_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineElastic : public Timeline
{
public:
    ~TimelineElastic();
    virtual FiniteTimeAction* getAction()override;
    
    float p;
    Timeline *child;
};

class TimelineElasticIn : public TimelineElastic
{
public:
    ~TimelineElasticIn();
    virtual FiniteTimeAction* getAction()override;
};

class TimelineElasticOut : public TimelineElastic
{
public:
    ~TimelineElasticOut();
    virtual FiniteTimeAction* getAction()override;
};

class TimelineElasticInOut : public TimelineElastic
{
public:
    ~TimelineElasticInOut();
    virtual FiniteTimeAction* getAction()override;
};

NS_EE_END

#endif /* TimelineElastic_hpp */
