//
//  TimelineSequence.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef TimelineSequence_hpp
#define TimelineSequence_hpp

#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineSequence : public Timeline
{
public:
    ~TimelineSequence();
    virtual FiniteTimeAction* getAction()override;
    
    std::vector<Timeline*> children;
};

NS_EE_END

#endif /* TimelineSequence_hpp */
