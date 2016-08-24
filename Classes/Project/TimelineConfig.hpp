//
//  TimelineConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/1.
//
//

#ifndef TimelineConfig_hpp
#define TimelineConfig_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;

class Timeline
{
public:
    virtual FiniteTimeAction* getAction(){return nullptr;};
    std::string id;
};

class TimelineMoveBy : public Timeline
{
public:
    virtual FiniteTimeAction* getAction()override;
    
    float duration;
    float x;
    float y;
};

class TimelineScaleTo : public Timeline
{
public:
    virtual FiniteTimeAction* getAction()override;
    
    float duration;
    float x;
    float y;
};

class TimelineSequence : public Timeline
{
public:
    ~TimelineSequence();
    virtual FiniteTimeAction* getAction()override;
    
    std::vector<Timeline*> children;
};

class TimelineRepeat : public Timeline
{
public:
    ~TimelineRepeat();
    virtual FiniteTimeAction* getAction()override;
    
    int repeat;
    Timeline* child;
};

class TimelineSpawn : public Timeline
{
public:
    ~TimelineSpawn();
    virtual FiniteTimeAction* getAction()override;
    
    std::vector<Timeline*> children;
};



NS_EE_END

#endif /* TimelineConfig_hpp */
