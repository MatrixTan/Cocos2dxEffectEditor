//
//  TimelineRepeat.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef TimelineRepeat_hpp
#define TimelineRepeat_hpp

#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineRepeat : public Timeline
{
public:
    ~TimelineRepeat();
    virtual FiniteTimeAction* getAction()override;
    
    int repeat;
    Timeline* child;
};

NS_EE_END

#endif /* TimelineRepeat_hpp */
