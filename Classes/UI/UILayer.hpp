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
#include "Common/NodeSingleton.hpp"
#include "ui/CocosGUI.h"
#include "UIProjectView.hpp"
#include "UIPropertyView.hpp"
#include "Project/ProjectConfig.hpp"
#include "Message/MessageParam.hpp"
#include "Message/MessageListener.hpp"
#include "UIDrawView.hpp"
#include "EngineEx/ShaderSprite.hpp"

NS_EE_BEGIN

USING_NS_CC;

enum class UI_STATE
{
    NONE = 0,
    PEN = 1
};

class UILayer : public NodeSingleton<UILayer> ,public Layer, public MessageListener
{
public:
    
    virtual bool init(void) override;
    UI_STATE getState(void);
    
    CREATE_FUNC(UILayer);
    
    void setStatus(const std::string& status);
    void loadProject(ProjectConfig* config);
    
private:
    void bindListener(void);
    void onUserTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onPenTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onSaveTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onLoadTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onNewTouchEvent(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    
    void onTest1Event(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onTest2Event(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    void onTest3Event(Ref *sender, cocos2d::ui::Widget::TouchEventType type);
    
    void onTouchBackground(Ref *sender, ui::Widget::TouchEventType type);
    
    void onSliderMessage1(void* sender, MessageParam *param);
    void onSliderMessage2(void* sender, MessageParam *param);
    void onSliderMessage3(void* sender, MessageParam *param);
    void setCurrentSprite(ShaderSprite *shader);
    
    Node *mContainer;
    UIProjectView *mProjectView;
    UIPropertyView *mPropertyView;
    ui::Text *mStatusText;
    UI_STATE mState;
    UIDrawView* mDrawPanel;
};

NS_EE_END

#endif /* UILayer_hpp */
