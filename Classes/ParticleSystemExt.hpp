//
//  ParticleSystemExt.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/10.
//
//

#ifndef ParticleSystemExt_hpp
#define ParticleSystemExt_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;

/*class ParticleFrameData
{
public:
    float time;
    
};*/

class ParticleSystemExt : public ParticleSystemQuad
{
public:
    static ParticleSystemExt* create(const std::string& filename);
    
    ParticleSystemExt();
    ~ParticleSystemExt();
    void setRadial(bool radial);
    void setFrameTile(int x, int y, float frameInterval);
    
    virtual void updateParticleQuads() override;
    virtual void update(float dt) override;
    bool initWithTotalParticles(int numberOfParticles);
    void addParticles(int count);
protected:
    void updatePosWithParticle(V3F_C4B_T2F_Quad *quad, const Vec2& newPosition,float size,float rotation);
    void resetFrameData(void);
    void updateParticleFrame(V3F_C4B_T2F_Quad *quad, int index);
    //径向粒子
    bool mbRadial;
    //粒子动画
    bool mbFrameTile;
    int mTilesX;
    int mTilesY;
    float mFrameInterval;
    float *mFrameTimes;

};


NS_EE_END

#endif /* ParticleSystemExt_hpp */
