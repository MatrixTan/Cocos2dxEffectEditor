//
//  TimelineMoveBy.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef TimelineMoveBy_hpp
#define TimelineMoveBy_hpp

#include <Timeline/Timeline.hpp>

NS_EE_BEGIN

class TimelineMoveBy : public Timeline
{
public:
    virtual FiniteTimeAction* getAction()override;
    
    float duration;
    float x;
    float y;
};

NS_EE_END

#endif /* TimelineMoveBy_hpp */
