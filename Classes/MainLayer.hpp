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
#include "NodeSingleton.hpp"
#include "ShaderSprite.hpp"

NS_EE_BEGIN

using namespace cocos2d;

class MainLayer : public NodeSingleton<MainLayer>, public Layer
{
public:
    MainLayer();
    ~MainLayer();
    virtual bool init(void);
    
    void addSprite(ShaderSprite *pSprite, int zorder = 0);
    void setBackground(const std::string& file);
    CREATE_FUNC(MainLayer);
private:
    
    bool onTouchBegin(Touch *touch, Event *event);
    void onTouchMove(Touch *touch, Event *event);
    void onTouchEnd(Touch *touch, Event *event);
    void onTouchCancel(Touch *touch, Event *event);
    
    Node *mContainer;
};

NS_EE_END

#endif /* MainLayer_hpp */
