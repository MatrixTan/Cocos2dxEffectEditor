//
//  ActionDrawBezierPath.hpp
//  EffectEditor
//
//  Created by Matrix on 2016/11/18.
//
//

#ifndef ActionDrawBezierPath_hpp
#define ActionDrawBezierPath_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include <cocos2d.h>
#include "BezierPathManager.hpp"

NS_EE_BEGIN
USING_NS_CC;

class ActionDrawBezierPath : public ActionInterval
{
public:
    static ActionDrawBezierPath* create(float duration,
                                        const BezierPointList& list,
                                        float width,
                                        Color4F color = Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    virtual void startWithTarget(cocos2d::Node* pTarget) override;
    virtual void step(float time) override;
    virtual ActionDrawBezierPath* clone() const override;

private:
    bool initWithDuration(float duration, const BezierPointList& list, float width, Color4F color);
    void drawBezierLine(BezierPoint* p1, BezierPoint* p2, float percent);

    BezierPointList mPoints;
    DrawNode* mDrawNode;
    float mDrawTime;
    std::list<float> mLengths;
    float mAllLength;
    float mWidth;
    Color4F mColor;
};

NS_EE_END

#endif /* ActionDrawBezierPath_hpp */
