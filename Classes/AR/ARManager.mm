//
//  ARManager.cpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/4.
//

#include "ARManager.hpp"

#import "ARViewController.h"
#include "AppController.h"
#include <cocos2d.h>
#import "platform/ios/CCEAGLView-ios.h"
#import "ARSceneViewController.h"

NS_EE_BEGIN

using namespace cocos2d;

void ARManager::showSceneView(void)
{
    ARSceneViewController* arview = [ARSceneViewController createView];
    [arview setCurrentProcess:1 maxNumber:3];
    [arview setGetRewardString:[NSString stringWithUTF8String:"获取奖励"]];
    AppController *appController = (AppController*)[[UIApplication sharedApplication] delegate];
    [appController.viewController presentViewController:arview animated:YES completion:nil];
}

void ARManager::stopSceneView()
{
    AppController *appController = (AppController *)[[UIApplication sharedApplication] delegate];
    [appController.viewController dismissViewControllerAnimated:NO completion:nil];
}

void ARManager::startVRView()
{
    ARViewController *arviewController = [ARViewController createView];
    AppController *appController = (AppController*)[[UIApplication sharedApplication] delegate];
    UIView* rootView = (UIView*)cocos2d::Director::getInstance()->getOpenGLView()->getEAGLView();
    [rootView addSubview: arviewController.view];
    mARView = arviewController;
    mARFrameInfo = new ARFrameInfo();
}

ARFrameInfo* ARManager::getARFrameInfo(){
    
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    EAGLContext* glContext = [((CCEAGLView*)glview->getEAGLView()) context];
    CVPixelBufferRef buffer =(CVPixelBufferRef) [(ARViewController*)mARView update:glContext];
    if(buffer){
        GLsizei width = (GLsizei)CVPixelBufferGetWidthOfPlane(buffer, 0);
        GLsizei height = (GLsizei)CVPixelBufferGetHeightOfPlane(buffer, 0);
        size_t dataSize = CVPixelBufferGetDataSize(buffer);
        
        void* dataY = CVPixelBufferGetBaseAddressOfPlane(buffer, 0);
        mARFrameInfo->textureY->initWithData(dataY, width * height, Texture2D::PixelFormat::I8, width, height, cocos2d::Size(width, height));
        
        size_t widthUV = CVPixelBufferGetWidthOfPlane(buffer, 1);
        size_t heightUV = CVPixelBufferGetHeightOfPlane(buffer, 1);
        void* dataUV = CVPixelBufferGetBaseAddressOfPlane(buffer, 1);
        mARFrameInfo->textureUV->initWithData(dataUV, widthUV * heightUV * 2.0, Texture2D::PixelFormat::AI88, widthUV, heightUV, cocos2d::Size(widthUV, heightUV));
        
        ARSession *arSession = [((ARViewController*)mARView) getSession];
        if(arSession != nullptr){
            matrix_float4x4 view = [arSession.currentFrame.camera viewMatrixForOrientation:UIInterfaceOrientationLandscapeRight];
            for(int i=0; i<4; i++){
                mARFrameInfo->view.m[i*4] = view.columns[i].x;
                mARFrameInfo->view.m[i*4+1] = view.columns[i].y;
                mARFrameInfo->view.m[i*4+2] = view.columns[i].z;
                mARFrameInfo->view.m[i*4+3] = view.columns[i].w;
            }
            
            /*
            matrix_float4x4 projection = [arSession.currentFrame.camera projectionMatrixForOrientation:UIInterfaceOrientationLandscapeRight viewportSize:CGSizeMake(1024, 768) zNear:0.1 zFar:1000];
            for(int i=0; i<4; i++){
                mARFrameInfo->projection.m[i*4] = projection.columns[i].x;
                mARFrameInfo->projection.m[i*4+1] = projection.columns[i].y;
                mARFrameInfo->projection.m[i*4+2] = projection.columns[i].z;
                mARFrameInfo->projection.m[i*4+3] = projection.columns[i].w;
            }
             */
        }
        
        return mARFrameInfo;
    }
    return nullptr;
}

NS_EE_END
