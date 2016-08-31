//
//  UIPropertyView.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/31.
//
//

#ifndef UIPropertyView_hpp
#define UIPropertyView_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

NS_EE_BEGIN

class UIPropertyView
{
public:
    UIPropertyView(Node* root);
    
protected:
    void onOpenButtonTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    Node* mRootNode;
    Button* mOpenButton;
    
    bool mOpened;
};

NS_EE_END

#endif /* UIPropertyView_hpp */
