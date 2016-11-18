//
//  Timeline.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#ifndef Timeline_hpp
#define Timeline_hpp

#include <stdio.h>
#include <cocos2d.h>
#include <string>
#include <vector>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;

class Timeline
{
public:
    virtual FiniteTimeAction* getAction(){return nullptr;};
    std::string id;
};

NS_EE_END

#endif /* Timeline_hpp */
