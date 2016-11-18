//
//  TimelineSpawn.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef TimelineSpawn_hpp
#define TimelineSpawn_hpp

#include "Timeline.hpp"

NS_EE_BEGIN

class TimelineSpawn : public Timeline
{
public:
    ~TimelineSpawn();
    virtual FiniteTimeAction* getAction()override;
    
    std::vector<Timeline*> children;
};

NS_EE_END

#endif /* TimelineSpawn_hpp */
