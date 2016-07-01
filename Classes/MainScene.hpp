//
//  MainScene.hpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#ifndef MainScene_hpp
#define MainScene_hpp

#include <stdio.h>
#include "EffectEditor.hpp"

using namespace cocos2d;

NS_EE_BEGIN

class MainScene : public Scene
{
public:
    virtual bool init() override;
    
    CREATE_FUNC(MainScene);
private:
    
    
};

NS_EE_END

#endif /* MainScene_hpp */
