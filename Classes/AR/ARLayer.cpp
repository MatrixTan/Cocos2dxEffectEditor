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
    
    mCameraSprite = ShaderSprite::create();
    addChild(mCameraSprite);
    //mCameraSprite->initWithFile("res/qrcode.jpg");
    mCameraSprite->initShader("shader/shader_default_vert.glsl", "shader/shader_ar_camera.glsl");
    Size winSize = Director::getInstance()->getWinSize();
    mCameraSprite->setPosition(Vec2(winSize.width*0.5f, winSize.height*0.5f));
    
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
    }
}



void ARLayer::onDraw(const cocos2d::Mat4 &transfrom, uint32_t flags)
{
    auto glProgramState = getGLProgramState();
    glProgramState->apply(transfrom);
    
    GL::blendFunc(BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst);
    GL::bindTexture2D(test->getName());
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);
    size_t startLocation = (size_t)&mQuad;
    
    int diff = offsetof(V3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(mQuad.bl), (void*)(startLocation+diff));
    
    diff = offsetof(V3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, sizeof(mQuad.bl), (void*)(startLocation+diff));
    
    diff = offsetof(V3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(mQuad.bl), (void*)(startLocation+diff));
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    CHECK_GL_ERROR_DEBUG();
    CC_INCREMENT_GL_DRAWN_BATCHES_AND_VERTICES(1,4);
}

NS_EE_END
