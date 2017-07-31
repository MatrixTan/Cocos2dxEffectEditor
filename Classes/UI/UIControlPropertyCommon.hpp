//
//  UIControlPropertyCommon.hpp
//  EffectEditor
//
//  Created by Matrix on 2017/3/1.
//
//

#ifndef UIControlPropertyCommon_hpp
#define UIControlPropertyCommon_hpp

#include <stdio.h>
#include "editor-support/cocostudio/CocoStudio.h"
#include "ShaderSprite.hpp"

using namespace cocos2d;
using namespace cocostudio;

NS_EE_BEGIN

class UIControlPropertyCommon : public Node{

public:
    UIControlPropertyCommon();
    void setCurrentSprite(ShaderSprite *sprite);
    
private:
    ui::Widget *mNode;
};


NS_EE_END

#endif /* UIControlPropertyCommon_hpp */
