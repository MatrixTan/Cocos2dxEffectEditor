//
//  ParticleSystemExt.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/10.
//
//

#include "ParticleSystemExt.hpp"

NS_EE_BEGIN

ParticleSystemExt * ParticleSystemExt::create(const std::string& filename)
{
    ParticleSystemExt *ret = new (std::nothrow) ParticleSystemExt();
    if (ret && ret->initWithFile(filename))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

ParticleSystemExt::ParticleSystemExt()
{
    mRadial = false;
}

void ParticleSystemExt::setRadial(bool radial)
{
    mRadial = radial;
    
}

void ParticleSystemExt::updatePosWithParticle(V3F_C4B_T2F_Quad *quad, const Vec2& newPosition,float size,float rotation)
{
    if(mRadial)
    {
        rotation = CC_RADIANS_TO_DEGREES(newPosition.getAngle(Vec2(0.0f, 1.0f)));
    }
    
    // vertices
    GLfloat size_2 = size/2;
    GLfloat x1 = -size_2;
    GLfloat y1 = -size_2;
    
    GLfloat x2 = size_2;
    GLfloat y2 = size_2;
    GLfloat x = newPosition.x;
    GLfloat y = newPosition.y;
    
    GLfloat r = (GLfloat)-CC_DEGREES_TO_RADIANS(rotation);
    GLfloat cr = cosf(r);
    GLfloat sr = sinf(r);
    GLfloat ax = x1 * cr - y1 * sr + x;
    GLfloat ay = x1 * sr + y1 * cr + y;
    GLfloat bx = x2 * cr - y1 * sr + x;
    GLfloat by = x2 * sr + y1 * cr + y;
    GLfloat cx = x2 * cr - y2 * sr + x;
    GLfloat cy = x2 * sr + y2 * cr + y;
    GLfloat dx = x1 * cr - y2 * sr + x;
    GLfloat dy = x1 * sr + y2 * cr + y;
    
    // bottom-left
    quad->bl.vertices.x = ax;
    quad->bl.vertices.y = ay;
    
    // bottom-right vertex:
    quad->br.vertices.x = bx;
    quad->br.vertices.y = by;
    
    // top-left vertex:
    quad->tl.vertices.x = dx;
    quad->tl.vertices.y = dy;
    
    // top-right vertex:
    quad->tr.vertices.x = cx;
    quad->tr.vertices.y = cy;
}

void ParticleSystemExt::updateParticleQuads()
{
    if (_particleCount <= 0) {
        return;
    }
    
    Vec2 currentPosition;
    if (_positionType == PositionType::FREE)
    {
        currentPosition = this->convertToWorldSpace(Vec2::ZERO);
    }
    else if (_positionType == PositionType::RELATIVE)
    {
        currentPosition = _position;
    }
    
    V3F_C4B_T2F_Quad *startQuad;
    Vec2 pos = Vec2::ZERO;
    if (_batchNode)
    {
        V3F_C4B_T2F_Quad *batchQuads = _batchNode->getTextureAtlas()->getQuads();
        startQuad = &(batchQuads[_atlasIndex]);
        pos = _position;
    }
    else
    {
        startQuad = &(_quads[0]);
    }
    
    if( _positionType == PositionType::FREE )
    {
        Vec3 p1(currentPosition.x, currentPosition.y, 0);
        Mat4 worldToNodeTM = getWorldToNodeTransform();
        worldToNodeTM.transformPoint(&p1);
        Vec3 p2;
        Vec2 newPos;
        float* startX = _particleData.startPosX;
        float* startY = _particleData.startPosY;
        float* x = _particleData.posx;
        float* y = _particleData.posy;
        float* s = _particleData.size;
        float* r = _particleData.rotation;
        V3F_C4B_T2F_Quad* quadStart = startQuad;
        for (int i = 0 ; i < _particleCount; ++i, ++startX, ++startY, ++x, ++y, ++quadStart, ++s, ++r)
        {
            p2.set(*startX, *startY, 0);
            worldToNodeTM.transformPoint(&p2);
            newPos.set(*x,*y);
            p2 = p1 - p2;
            newPos.x -= p2.x - pos.x;
            newPos.y -= p2.y - pos.y;
            updatePosWithParticle(quadStart, newPos, *s, *r);
        }
    }
    else if( _positionType == PositionType::RELATIVE )
    {
        Vec2 newPos;
        float* startX = _particleData.startPosX;
        float* startY = _particleData.startPosY;
        float* x = _particleData.posx;
        float* y = _particleData.posy;
        float* s = _particleData.size;
        float* r = _particleData.rotation;
        V3F_C4B_T2F_Quad* quadStart = startQuad;
        for (int i = 0 ; i < _particleCount; ++i, ++startX, ++startY, ++x, ++y, ++quadStart, ++s, ++r)
        {
            newPos.set(*x, *y);
            newPos.x = *x - (currentPosition.x - *startX);
            newPos.y = *y - (currentPosition.y - *startY);
            newPos += pos;
            updatePosWithParticle(quadStart, newPos, *s, *r);
        }
    }
    else
    {
        Vec2 newPos;
        float* startX = _particleData.startPosX;
        float* startY = _particleData.startPosY;
        float* x = _particleData.posx;
        float* y = _particleData.posy;
        float* s = _particleData.size;
        float* r = _particleData.rotation;
        V3F_C4B_T2F_Quad* quadStart = startQuad;
        for (int i = 0 ; i < _particleCount; ++i, ++startX, ++startY, ++x, ++y, ++quadStart, ++s, ++r)
        {
            newPos.set(*x + pos.x, *y + pos.y);
            updatePosWithParticle(quadStart, newPos, *s, *r);
        }
    }
    
    //set color
    if(_opacityModifyRGB)
    {
        V3F_C4B_T2F_Quad* quad = startQuad;
        float* r = _particleData.colorR;
        float* g = _particleData.colorG;
        float* b = _particleData.colorB;
        float* a = _particleData.colorA;
        
        for (int i = 0; i < _particleCount; ++i,++quad,++r,++g,++b,++a)
        {
            GLubyte colorR = *r * *a * 255;
            GLubyte colorG = *g * *a * 255;
            GLubyte colorB = *b * *a * 255;
            GLubyte colorA = *a * 255;
            quad->bl.colors.set(colorR, colorG, colorB, colorA);
            quad->br.colors.set(colorR, colorG, colorB, colorA);
            quad->tl.colors.set(colorR, colorG, colorB, colorA);
            quad->tr.colors.set(colorR, colorG, colorB, colorA);
        }
    }
    else
    {
        V3F_C4B_T2F_Quad* quad = startQuad;
        float* r = _particleData.colorR;
        float* g = _particleData.colorG;
        float* b = _particleData.colorB;
        float* a = _particleData.colorA;
        
        for (int i = 0; i < _particleCount; ++i,++quad,++r,++g,++b,++a)
        {
            GLubyte colorR = *r * 255;
            GLubyte colorG = *g * 255;
            GLubyte colorB = *b * 255;
            GLubyte colorA = *a * 255;
            quad->bl.colors.set(colorR, colorG, colorB, colorA);
            quad->br.colors.set(colorR, colorG, colorB, colorA);
            quad->tl.colors.set(colorR, colorG, colorB, colorA);
            quad->tr.colors.set(colorR, colorG, colorB, colorA);
        }
    }
}



NS_EE_END