//
//  PostRenderEffectLayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/7/8.
//
//

#include "PostRenderEffectLayer.hpp"
#include "MainLayer.hpp"

NS_EE_BEGIN

PostRenderEffectLayer::PostRenderEffectLayer()
:mRenderTexture(nullptr)
,mRunTime(0.0f)
,mRunning(false)
{
}

PostRenderEffectLayer::~PostRenderEffectLayer()
{
    CC_SAFE_RELEASE(mRenderTexture);
}

bool PostRenderEffectLayer::init()
{
    auto winSize = Director::getInstance()->getWinSize();
    mRenderTexture = RenderTexture::create(winSize.width, winSize.height, Texture2D::PixelFormat::RGBA8888);
    CC_SAFE_RETAIN(mRenderTexture);
    
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
    mQuad.bl.texCoords.u = 0.0f;
    mQuad.bl.texCoords.v = 0.0f;
    mQuad.br.texCoords.u = 1.0f;
    mQuad.br.texCoords.v = 0.0f;
    mQuad.tl.texCoords.u = 0.0f;
    mQuad.tl.texCoords.v = 1.0f;
    mQuad.tr.texCoords.u = 1.0f;
    mQuad.tr.texCoords.v = 1.0f;
    
    auto program = GLProgram::createWithFilenames("shader/shader_post_render_vert.glsl", "shader/shader_post_render_frag.glsl");
    auto programState = GLProgramState::getOrCreateWithGLProgram(program);
    setGLProgramState(programState);
    scheduleUpdate();
    Layer::init();
    return true;
}

void PostRenderEffectLayer::setDrawRect(const cocos2d::Rect &rect, float duration)
{
    if(mRunning)
    {
        return;
    }
    
    mDrawRect = rect;
    auto winSize = Director::getInstance()->getWinSize();
    float l = (mDrawRect.getMinX() - winSize.width * 0.5f)/winSize.width * 2.0f;
    float r = (mDrawRect.getMaxX() - winSize.width * 0.5f)/winSize.width * 2.0f;
    float b = (mDrawRect.getMinY() - winSize.height * 0.5f)/winSize.height * 2.0f;
    float t = (mDrawRect.getMaxY() - winSize.height * 0.5f)/winSize.height * 2.0f;
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
    
    getGLProgramState()->setUniformVec2("center", Vec2((l+r)*0.5f, (b+t)*0.5f));
    getGLProgramState()->setUniformVec2("winsize", winSize);
    
    mRunning = true;
    mRunTime = 0.0f;
    if(duration > 0.0f)
    {
        scheduleOnce([&](float dt){
            this->onTimeout();
        }, duration, "disable_post_render_effect");
    }
}

void PostRenderEffectLayer::onTimeout()
{
    mRunning = false;
}

void PostRenderEffectLayer::update(float dt)
{
    mRunTime += dt;
}

void PostRenderEffectLayer::draw(cocos2d::Renderer *render, const cocos2d::Mat4 &transform, uint32_t flags)
{
    if(mRunning && mRenderTexture)
    {
        mRenderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 1.0f);
        MainLayer::getInstance()->visit(render, transform, flags);
        mRenderTexture->end();
        
        mRenderCommand.init(_globalZOrder, transform, flags);
        mRenderCommand.func = CC_CALLBACK_0(PostRenderEffectLayer::onDraw, this, transform, flags);
        render->addCommand(&mRenderCommand);
        
    }
}

void PostRenderEffectLayer::onDraw(const cocos2d::Mat4 &transfrom, uint32_t flags)
{
    
    auto glProgramState = getGLProgramState();
    glProgramState->apply(transfrom);
    glProgramState->setUniformFloat("time", mRunTime);
    
    GL::blendFunc(BlendFunc::DISABLE.src, BlendFunc::DISABLE.dst);
    GL::bindTexture2D(mRenderTexture->getSprite()->getTexture()->getName());
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

NS_EE_END;