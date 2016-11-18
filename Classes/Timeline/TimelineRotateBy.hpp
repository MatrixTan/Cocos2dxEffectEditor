//
//  TimelineRotateBy.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef TimelineRotateBy_hpp
#define TimelineRotateBy_hpp

#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineRotateBy : public Timeline
{
public:
    virtual FiniteTimeAction* getAction()override;
    
    float duration;
    float angle;
};

NS_EE_END

#endif /* TimelineRotateBy_hpp */
