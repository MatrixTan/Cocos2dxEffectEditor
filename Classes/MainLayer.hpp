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
    
    void addSprite(const std::string& id, ShaderSprite *pSprite, int zorder = 0);
    ShaderSprite* getSprite(const std::string& id);
    void setBackground(const std::string& file);
    CREATE_FUNC(MainLayer);
private:
    
    bool onTouchBegin(Touch *touch, Event *event);
    void onTouchMove(Touch *touch, Event *event);
    void onTouchEnd(Touch *touch, Event *event);
    void onTouchCancel(Touch *touch, Event *event);
    
    std::map<std::string, ShaderSprite*> mSprites;
};

NS_EE_END

#endif /* MainLayer_hpp */
