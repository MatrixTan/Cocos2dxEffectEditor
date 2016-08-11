//
//  DrawLineLayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/11.
//
//

#ifndef DrawLineLayer_hpp
#define DrawLineLayer_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;

class DrawLineLayer: public Layer
{
public:
    static DrawLineLayer* create(void);
    
    virtual bool init() override;

};


NS_EE_END


#endif /* DrawLineLayer_hpp */
