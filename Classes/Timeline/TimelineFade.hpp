//
//  TimelineFade.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/12/6.
//
//

#ifndef TimelineFade_hpp
#define TimelineFade_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineFade : public Timeline
{
public:
    ~TimelineFade();
    
    virtual FiniteTimeAction* getAction()override;
    
    float to;
    float duration;
};

NS_EE_END

#endif /* TimelineFade_hpp */
