//
//  ActionEx.hpp
//  EffectEditor
//
//  Created by Matrix on 16/10/18.
//
//

#ifndef ActionEx_hpp
#define ActionEx_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include <cocos2d.h>

NS_EE_BEGIN

USING_NS_CC;

class BezierRotateTo : public BezierTo
{
public:
    static BezierRotateTo* create(float duration, const cocos2d::ccBezierConfig& config);
    
    virtual void startWithTarget(cocos2d::Node* pTarget) override;
    virtual void update(float time) override;
    virtual BezierRotateTo* clone() const override;
private:
    Vec2 mLastPos;
};

NS_EE_END


#endif /* ActionEx_hpp */
