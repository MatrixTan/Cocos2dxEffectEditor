//
//  UIProjectView.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/24.
//
//

#ifndef UIProjectView_hpp
#define UIProjectView_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

NS_EE_BEGIN

class UIProjectView
{
public:
    UIProjectView(Node* root);
    
protected:
    void onOpenButtonTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    
private:
    Node* mRootNode;
    Button* mOpenButton;
    
    bool mOpened;
};

NS_EE_END

#endif /* UIProjectView_hpp */
