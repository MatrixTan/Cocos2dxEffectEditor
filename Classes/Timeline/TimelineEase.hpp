//
//  TimelineEase.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/5.
//
//

#ifndef TimelineEase_hpp
#define TimelineEase_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineEase : public Timeline
{
public:
    ~TimelineEase();
    virtual FiniteTimeAction* getAction()override;
    
    float rate;
    Timeline *child;
};

NS_EE_END


#endif /* TimelineEase_hpp */
