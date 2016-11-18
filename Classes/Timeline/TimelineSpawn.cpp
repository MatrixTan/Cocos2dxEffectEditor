//
//  TimelineSpawn.cpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/1.
//
//

#include "TimelineSpawn.hpp"

NS_EE_BEGIN

//==========TimelineSpawn===============
TimelineSpawn::~TimelineSpawn()
{
    for(auto child : children){
        CC_SAFE_DELETE(child);
    }
}

FiniteTimeAction* TimelineSpawn::getAction(){
    Vector<FiniteTimeAction*> actions;
    for(auto child : children){
        actions.pushBack(child->getAction());
    }
    return Spawn::create(actions);
}

NS_EE_END