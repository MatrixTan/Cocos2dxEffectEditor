//
//  BackgroundConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/28.
//
//

#ifndef BackgroundConfig_hpp
#define BackgroundConfig_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include <cocos2d.h>

USING_NS_CC;

NS_EE_BEGIN

class BackgroundConfig
{
public:
    BackgroundConfig();
    
    std::string file;
    Vec2 scale;
};

NS_EE_END

#endif /* BackgroundConfig_hpp */
