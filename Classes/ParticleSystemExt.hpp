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

class ParticleSystemExt : public ParticleSystemQuad
{
public:
    static ParticleSystemExt* create(const std::string& filename);
    
    ParticleSystemExt();
    
    void setRadial(bool radial);
    virtual void updateParticleQuads() override;
    
protected:
    void updatePosWithParticle(V3F_C4B_T2F_Quad *quad, const Vec2& newPosition,float size,float rotation);
    bool mRadial;

};


NS_EE_END

#endif /* ParticleSystemExt_hpp */
