//
//  UILayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/26.
//
//

#ifndef UILayer_hpp
#define UILayer_hpp

#include <stdio.h>
#include <cocos2d.h>
#include "GlobalDefine.hpp"
#include "NodeSingleton.hpp"
#include "ui/CocosGUI.h"

NS_EE_BEGIN

USING_NS_CC;

enum class UI_STATE
{
    NONE = 0,
    PEN = 1
};

class UILayer : public NodeSingleton<UILayer> ,public Layer
{
public:
    
    virtual bool init(void) override;
    UI_STATE getState(void);
    
    CREATE_FUNC(UILayer);
    
private:
    
    void onUserTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onPenTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    
    Node *mContainer;
    UI_STATE mState;
};

NS_EE_END

#endif /* UILayer_hpp */
