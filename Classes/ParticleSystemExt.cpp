//
//  ParticleSystemExt.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/10.
//
//

#include "ParticleSystemExt.hpp"

NS_EE_BEGIN

inline void nomalize_point(float x, float y, particle_point* out)
{
    float n = x * x + y * y;
    // Already normalized.
    if (n == 1.0f)
        return;
    
    n = sqrt(n);
    // Too close to zero.
    if (n < MATH_TOLERANCE)
        return;
    
    n = 1.0f / n;
    out->x = x * n;
    out->y = y * n;
}



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

ParticleSystemExt* ParticleSystemExt::create(cocos2d::ValueMap &dictionary, const std::string &dirname)
{
    ParticleSystemExt *ret = new (std::nothrow) ParticleSystemExt();
    if (ret && ret->initWithDictionary(dictionary, dirname))
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return ret;
}

ParticleSystemExt::ParticleSystemExt()
:mbRadial(false),
mbFrameTile(false),
mTilesX(1),
mTilesY(2),
mFrameInterval(0.0f),
mFrameTimes(nullptr),
mbRandomFrame(false),
mFrameIndex(nullptr)
{
    
}

ParticleSystemExt::~ParticleSystemExt()
{
    /////EXT CODE///////////
    if(mFrameTimes)
    {
        delete [] mFrameTimes;
        mFrameTimes = nullptr;
    }
    /////////////////////////
}

/////EXT CODE///////////
void ParticleSystemExt::setRadial(bool radial)
{
    mbRadial = radial;
}

void ParticleSystemExt::setFrameTile(int x, int y, float frameInterval)
{
    mbFrameTile = true;
    mTilesX = x;
    mTilesY = y;
    mFrameInterval = frameInterval;
    resetFrameData();
}

void ParticleSystemExt::resetFrameData()
{
    if(mFrameTimes != nullptr){
        delete[] mFrameTimes;
        mFrameTimes = nullptr;
    }
    mFrameTimes = new float[_totalParticles];
    memset(mFrameTimes, 0, sizeof(float)*_totalParticles);
}


void ParticleSystemExt::setRandomFrame(int x, int y)
{
    mbRandomFrame = true;
    mTilesX = x;
    mTilesY = y;
    resetRandomFrameData();
}

void ParticleSystemExt::resetRandomFrameData()
{
    if(mFrameIndex != nullptr){
        delete [] mFrameIndex;
        mFrameIndex = nullptr;
    }
    mFrameIndex = new int[_totalParticles];
    memset(mFrameIndex, 0, sizeof(int) * _totalParticles);
}


bool ParticleSystemExt::initWithTotalParticles(int numberOfParticles)
{
    bool ret = ParticleSystemQuad::initWithTotalParticles(numberOfParticles);
    if(mbFrameTile){
        resetFrameData();
    }
    if(mbRandomFrame){
        resetRandomFrameData();
    }
    return ret;
}


void ParticleSystemExt::addParticles(int count)
{
    int start = _particleCount;
    ParticleSystem::addParticles(count);
    if(mbFrameTile){
        for(int i=start ; i<_particleCount; i++){
            mFrameTimes[i] = 0.0f;
        }
    }
    if(mbRandomFrame){
        for(int i= start; i<_particleCount; i++){
            mFrameIndex[i] = cocos2d::random(0, mTilesX * mTilesY-1);
        }
    }
    
}

void ParticleSystemExt::updateParticleFrame(cocos2d::V3F_C4B_T2F_Quad *quad, int index)
{
    float width = 1.0f/mTilesX, height = 1.0f/mTilesY;
    int x = 0, y = 0;
    if(mbFrameTile){
        float time = mFrameTimes[index];
        int frameIndex = floor(time / mFrameInterval);
        
        if(frameIndex > mTilesX * mTilesY){
            mFrameTimes[index] = time - frameIndex * mFrameInterval;
        }else{
            x = frameIndex % mTilesX;
            y = frameIndex / mTilesX;
        }

    }
    
    if(mbRandomFrame){
        x = mFrameIndex[index] % mTilesX;
        y = mFrameIndex[index] / mTilesX;
    }
    quad->bl.texCoords.u = width * x;
    quad->bl.texCoords.v = height * (y + 1);
    quad->br.texCoords.u = width * (x + 1);
    quad->br.texCoords.v = quad->bl.texCoords.v;
    quad->tl.texCoords.u = quad->bl.texCoords.u;
    quad->tl.texCoords.v = height * y;
    quad->tr.texCoords.u = quad->br.texCoords.u;
    quad->tr.texCoords.v = quad->tl.texCoords.v;
}

////////////////////////

void ParticleSystemExt::updatePosWithParticle(V3F_C4B_T2F_Quad *quad, const Vec2& newPosition,float size,float rotation)
{
    ///////////EXT CODE//////////////
    if(mbRadial)
    {
        rotation = CC_RADIANS_TO_DEGREES(newPosition.getAngle(Vec2(0.0f, 1.0f)));
    }
    /////////////////////////////////
    
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
    
    ////////////EXT CODE/////////////
    if(mbFrameTile || mbRandomFrame){
        V3F_C4B_T2F_Quad* quad = startQuad;
        for(int i=0; i<_particleCount; ++i, ++quad){
            updateParticleFrame(quad, i);
        }
    }
    /////////////////////////////////
}

void ParticleSystemExt::update(float dt)
{
    CC_PROFILER_START_CATEGORY(kProfilerCategoryParticles , "CCParticleSystem - update");
    
    if (_isActive && _emissionRate)
    {
        float rate = 1.0f / _emissionRate;
        //issue #1201, prevent bursts of particles, due to too high emitCounter
        if (_particleCount < _totalParticles)
        {
            _emitCounter += dt;
            if (_emitCounter < 0.f)
                _emitCounter = 0.f;
        }
        
        int emitCount = MIN(_totalParticles - _particleCount, _emitCounter / rate);
        addParticles(emitCount);
        _emitCounter -= rate * emitCount;
        
        _elapsed += dt;
        if (_elapsed < 0.f)
            _elapsed = 0.f;
        if (_duration != DURATION_INFINITY && _duration < _elapsed)
        {
            this->stopSystem();
        }
    }
    
    {
        for (int i = 0; i < _particleCount; ++i)
        {
            _particleData.timeToLive[i] -= dt;
        }
        
        for (int i = 0; i < _particleCount; ++i)
        {
            if (_particleData.timeToLive[i] <= 0.0f)
            {
                int j = _particleCount - 1;
                while (j > 0 && _particleData.timeToLive[j] <= 0)
                {
                    _particleCount--;
                    j--;
                }
                _particleData.copyParticle(i, _particleCount - 1);
                ////////////EXT CODE/////////////
                if(mbFrameTile){
                    mFrameTimes[i] = mFrameTimes[_particleCount-1];
                }
                if(mbRandomFrame){
                    mFrameIndex[i] = mFrameIndex[_particleCount-1];
                }
                /////////////////////////////////
                if (_batchNode)
                {
                    //disable the switched particle
                    int currentIndex = _particleData.atlasIndex[i];
                    _batchNode->disableParticle(_atlasIndex + currentIndex);
                    //switch indexes
                    _particleData.atlasIndex[_particleCount - 1] = currentIndex;
                }
                --_particleCount;
                if( _particleCount == 0 && _isAutoRemoveOnFinish )
                {
                    this->unscheduleUpdate();
                    _parent->removeChild(this, true);
                    return;
                }
            }
        }
        
        /////////////EXT CODE//////////////
        if(mbFrameTile){
            for(int i=0; i<_particleCount; i++){
                mFrameTimes[i] += dt;
            }
        }
        ///////////////////////////////////
        
        if (_emitterMode == Mode::GRAVITY)
        {
            for (int i = 0 ; i < _particleCount; ++i)
            {
                particle_point tmp, radial = {0.0f, 0.0f}, tangential;
                
                // radial acceleration
                if (_particleData.posx[i] || _particleData.posy[i])
                {
                    nomalize_point(_particleData.posx[i], _particleData.posy[i], &radial);
                }
                tangential = radial;
                radial.x *= _particleData.modeA.radialAccel[i];
                radial.y *= _particleData.modeA.radialAccel[i];
                
                // tangential acceleration
                std::swap(tangential.x, tangential.y);
                tangential.x *= - _particleData.modeA.tangentialAccel[i];
                tangential.y *= _particleData.modeA.tangentialAccel[i];
                
                // (gravity + radial + tangential) * dt
                tmp.x = radial.x + tangential.x + modeA.gravity.x;
                tmp.y = radial.y + tangential.y + modeA.gravity.y;
                tmp.x *= dt;
                tmp.y *= dt;
                
                _particleData.modeA.dirX[i] += tmp.x;
                _particleData.modeA.dirY[i] += tmp.y;
                
                // this is cocos2d-x v3.0
                // if (_configName.length()>0 && _yCoordFlipped != -1)
                
                // this is cocos2d-x v3.0
                tmp.x = _particleData.modeA.dirX[i] * dt * _yCoordFlipped;
                tmp.y = _particleData.modeA.dirY[i] * dt * _yCoordFlipped;
                _particleData.posx[i] += tmp.x;
                _particleData.posy[i] += tmp.y;
            }
        }
        else
        {
            for (int i = 0; i < _particleCount; ++i)
            {
                _particleData.modeB.angle[i] += _particleData.modeB.degreesPerSecond[i] * dt;
            }
            
            for (int i = 0; i < _particleCount; ++i)
            {
                _particleData.modeB.radius[i] += _particleData.modeB.deltaRadius[i] * dt;
            }
            
            for (int i = 0; i < _particleCount; ++i)
            {
                _particleData.posx[i] = - cosf(_particleData.modeB.angle[i]) * _particleData.modeB.radius[i];
            }
            for (int i = 0; i < _particleCount; ++i)
            {
                _particleData.posy[i] = - sinf(_particleData.modeB.angle[i]) * _particleData.modeB.radius[i] * _yCoordFlipped;
            }
        }
        
        //color r,g,b,a
        for (int i = 0 ; i < _particleCount; ++i)
        {
            _particleData.colorR[i] += _particleData.deltaColorR[i] * dt;
        }
        
        for (int i = 0 ; i < _particleCount; ++i)
        {
            _particleData.colorG[i] += _particleData.deltaColorG[i] * dt;
        }
        
        for (int i = 0 ; i < _particleCount; ++i)
        {
            _particleData.colorB[i] += _particleData.deltaColorB[i] * dt;
        }
        
        for (int i = 0 ; i < _particleCount; ++i)
        {
            _particleData.colorA[i] += _particleData.deltaColorA[i] * dt;
        }
        //size
        for (int i = 0 ; i < _particleCount; ++i)
        {
            _particleData.size[i] += (_particleData.deltaSize[i] * dt);
            _particleData.size[i] = MAX(0, _particleData.size[i]);
        }
        //angle
        for (int i = 0 ; i < _particleCount; ++i)
        {
            _particleData.rotation[i] += _particleData.deltaRotation[i] * dt;
        }
        
        updateParticleQuads();
        _transformSystemDirty = false;
    }
    
    // only update gl buffer when visible
    if (_visible && ! _batchNode)
    {
        postStep();
    }
    
    CC_PROFILER_STOP_CATEGORY(kProfilerCategoryParticles , "CCParticleSystem - update");

}


NS_EE_END