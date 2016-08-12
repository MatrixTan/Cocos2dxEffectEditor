//
//  DrawLineLayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/11.
//
//

#ifndef DrawLineLayer_hpp
#define DrawLineLayer_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Common/NodeSingleton.hpp"

NS_EE_BEGIN

USING_NS_CC;

class BezierPoint
{
public:
    Vec2 pos;
    Vec2 control1;
    Vec2 control2;
};

class BezierLine
{
public:
    BezierPoint* start;
    BezierPoint* end;
};

class DrawLineLayer: public NodeSingleton<DrawLineLayer>, public Layer
{
public:
    static DrawLineLayer* create(void);
    
    virtual bool init() override;

    bool onTouchBegin(Touch *touch, Event *event);
    void onTouchMove(Touch *touch, Event *event);
    void onTouchEnd(Touch *touch, Event *event);
    void onTouchCancel(Touch *touch, Event *event);
    
private:
    
};


NS_EE_END


#endif /* DrawLineLayer_hpp */
