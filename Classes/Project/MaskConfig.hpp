//
//  MaskConfig.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/12.
//
//

#ifndef MaskConfig_hpp
#define MaskConfig_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;

class MaskConfig
{
public:
    MaskConfig();
    
    std::string id;
    std::string stencil;
    Vec2 offset;
    Vec2 scale;
    float alphaThreshold;
};

NS_EE_END

#endif /* MaskConfig_hpp */
