//
//  MainLayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#ifndef MainLayer_hpp
#define MainLayer_hpp

#include <stdio.h>
#include "EffectEditor.hpp"
#include "GlobalDefine.hpp"

NS_EE_BEGIN

using namespace cocos2d;

class MainLayer : public Layer
{
public:
    MainLayer();
    ~MainLayer();
    virtual bool init(void);
    
    void setSprite(const std::string& file);
    CREATE_FUNC(MainLayer);
    
private:
    Node *mContainer;
};

NS_EE_END

#endif /* MainLayer_hpp */
