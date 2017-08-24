//
//  ARCamera.cpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/23.
//

#include "ARCamera.hpp"

NS_EE_BEGIN

const Mat4& ARCamera::getViewProjectionMatrix(void) const
{
    Mat4::multiply(_projection, _view, &_viewProjection);
    return _viewProjection;
}

void ARCamera::setViewMatrix(const cocos2d::Mat4 &view){
    _view = view;
}

void ARCamera::setProjectionMatrix(const cocos2d::Mat4 &project){
    mProjectionMatrix = project;
}

NS_EE_END
