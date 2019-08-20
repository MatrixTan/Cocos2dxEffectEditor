//
//  TimelineScaleTo.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef TimelineScaleTo_hpp
#define TimelineScaleTo_hpp

#include "Timeline/Timeline.hpp"

NS_EE_BEGIN

class TimelineScaleTo : public Timeline
{
public:
    virtual FiniteTimeAction* getAction()override;
    
    float duration;
    float x;
    float y;
};

NS_EE_END

#endif /* TimelineScaleTo_hpp */
