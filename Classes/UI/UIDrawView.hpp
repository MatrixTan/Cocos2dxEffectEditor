//
//  UIDrawView.hpp
//  EffectEditor
//
//  Created by Matrix on 16/10/18.
//
//

#ifndef UIDrawView_hpp
#define UIDrawView_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include <list>
#include "BezierPathManager.hpp"

USING_NS_CC;
NS_EE_BEGIN


enum E_SELECT_POINT_TYPE
{
    NONE = 0,
    SRC = 1,
    CONTROL1 = 2,
    CONTROL2 = 3
};

class UIDrawView
{
public:
    UIDrawView(Node* root);
    
    void onTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onSaveEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void setVisible(bool visible);
    void savePath(void);
    
private:
    void reDraw(void);
    bool checkSelectPoint(const Vec2& pos);
    
private:
    ui::Widget* mRoot;
    DrawNode* mDrawNode;
    std::list<BezierPoint*> mBezierPoints;
    BezierPoint* mCurrentBezierPoint;
    E_SELECT_POINT_TYPE mSelectType;
    BezierPoint* mSelectedPoint;
};

NS_EE_END

#endif /* UIDrawView_hpp */
