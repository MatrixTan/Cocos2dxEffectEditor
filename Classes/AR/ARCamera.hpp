//
//  ARCamera.hpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/23.
//

#ifndef ARCamera_hpp
#define ARCamera_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;

class ARCamera : public Camera
{
public:
    virtual const Mat4& getViewProjectionMatrix() const override;
    void setViewMatrix(const Mat4& view);
    void setProjectionMatrix(const Mat4& project);
private:
    Mat4 mViewMatrix;
    Mat4 mProjectionMatrix;
};

NS_EE_END

#endif /* ARCamera_hpp */
