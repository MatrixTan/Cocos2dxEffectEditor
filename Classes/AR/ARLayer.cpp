//
//  ARLayer.cpp
//  EffectEditor-mobile
//
//  Created by Matrix on 2017/8/9.
//

#include "ARLayer.hpp"
#include "ARManager.hpp"

NS_EE_BEGIN

bool ARLayer::init()
{
    float l = -1.0f, b = -1.0f, t = 1.0f, r = 1.0f;
    
    memset(&mQuad, 0, sizeof(mQuad));
    mQuad.bl.colors = Color4B::WHITE;
    mQuad.br.colors = Color4B::WHITE;
    mQuad.tl.colors = Color4B::WHITE;
    mQuad.tr.colors = Color4B::WHITE;
    mQuad.bl.vertices.set(l, b, 0.0f);
    mQuad.br.vertices.set(r, b, 0.0f);
    mQuad.tl.vertices.set(l, t, 0.0f);
    mQuad.tr.vertices.set(r, t, 0.0f);
    mQuad.bl.texCoords.u = l * 0.5f + 0.5f;
    mQuad.bl.texCoords.v = b * 0.5f + 0.5f;
    mQuad.br.texCoords.u = r * 0.5f + 0.5f;
    mQuad.br.texCoords.v = b * 0.5f + 0.5f;
    mQuad.tl.texCoords.u = l * 0.5f + 0.5f;
    mQuad.tl.texCoords.v = t * 0.5f + 0.5f;
    mQuad.tr.texCoords.u = r * 0.5f + 0.5f;
    mQuad.tr.texCoords.v = t * 0.5f + 0.5f;
    
    ARManager::getInstance()->startVRView();
    Size winSize = Director::getInstance()->getWinSize();
    
    auto backgroundCamera = Camera::createOrthographic(winSize.width, winSize.height, 1, 1000);
    backgroundCamera->setCameraFlag(CameraFlag::USER2);
    backgroundCamera->setPositionZ(40);
    addChild(backgroundCamera);

    mCameraSprite = ShaderSprite::create();
    addChild(mCameraSprite);
    mCameraSprite->initShader("shader/shader_default_vert.glsl", "shader/shader_ar_camera.glsl");
    mCameraSprite->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
    mCameraSprite->setCameraMask((unsigned)4);

    Size visibleSize = Director::getInstance()->getVisibleSize();
    mARCamera = new ARCamera();//Camera::createPerspective(60, visibleSize.width/visibleSize.height, 0.1f, 200.0f);
    mARCamera->initPerspective(60, visibleSize.width/visibleSize.height, 0.001f, 200.0f);
    mARCamera->autorelease();
    mARCamera->setCameraFlag(CameraFlag::USER3);
    mARCamera->setPosition3D(Vec3(0.0f, 0.0f, 30.0f));
    addChild(mARCamera);
    
    Sprite3D *sprite3D = Sprite3D::create("res/axe.c3b", "res/arx.png");
    addChild(sprite3D, 2);
    sprite3D->setScale(0.01);
    sprite3D->setPosition3D(Vec3(0.0f, 0.0f, 0.01f));
    sprite3D->setCameraMask((unsigned)8);
    
    scheduleUpdate();
    test = Director::getInstance()->getTextureCache()->addImage("res/qrcode.jpg");
    return Layer::init();
}

void ARLayer::update(float dt)
{
    ARFrameInfo *arInfo = ARManager::getInstance()->getARFrameInfo();
    if(arInfo){
        mCameraSprite->setTexture(arInfo->textureY);
        Size winSize = Director::getInstance()->getWinSize();
        Size textSize = arInfo->textureY->getContentSize();
        mCameraSprite->setScale(winSize.width/textSize.width, winSize.height/textSize.height);
        mCameraSprite->setTextureRect(Rect(0, 0, textSize.width, textSize.height));
        mCameraSprite->getGLProgramState()->setUniformTexture("u_textureUV", arInfo->textureUV);
        
        mARCamera->setViewMatrix(arInfo->view);
    }
}


NS_EE_END
