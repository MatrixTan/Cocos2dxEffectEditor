//
//  TimelineDelay.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/6.
//
//

#ifndef TimelineDelay_hpp
#define TimelineDelay_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineDelay : public Timeline
{
public:
    virtual FiniteTimeAction* getAction()override;
    
    float delay;
};

NS_EE_END

#endif /* TimelineDelay_hpp */
