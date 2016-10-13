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
#include <cocos2d.h>
#include "GlobalDefine.hpp"

USING_NS_CC;

NS_EE_BEGIN



/*class ParticleFrameData
{
public:
    float time;
    
};*/

class ParticleSystemExt : public ParticleSystemQuad
{
public:
    static ParticleSystemExt* create(const std::string& filename);
    static ParticleSystemExt* create(cocos2d::ValueMap &dictionary, const std::string &dirname);
    
    ParticleSystemExt();
    ~ParticleSystemExt();
    void setRadial(bool radial);
    void setFrameTile(int x, int y, float frameInterval);
    void setRandomFrame(int x, int y);
    
    virtual void updateParticleQuads() override;
    virtual void update(float dt) override;
    bool initWithTotalParticles(int numberOfParticles);
    void addParticles(int count);
protected:
    void updatePosWithParticle(V3F_C4B_T2F_Quad *quad, const Vec2& newPosition,float size,float rotation);
    void resetFrameData(void);
    void resetRandomFrameData(void);
    void updateParticleFrame(V3F_C4B_T2F_Quad *quad, int index);
    //径向粒子
    bool mbRadial;
    //粒子动画
    bool mbFrameTile;
    int mTilesX;
    int mTilesY;
    float mFrameInterval;
    float *mFrameTimes;
    //随机贴图
    bool mbRandomFrame;
    int *mFrameIndex;

};


NS_EE_END

#endif /* ParticleSystemExt_hpp */
