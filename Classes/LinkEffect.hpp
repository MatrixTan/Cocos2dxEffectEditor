//
//  LinkEffect.hpp
//  EffectEditor
//
//  Created by Matrix on 16/7/27.
//
//

#ifndef LinkEffect_hpp
#define LinkEffect_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include <cocos2d.h>

NS_EE_BEGIN

USING_NS_CC;

enum class LINK_EFFECT_STATE
{
    NONE,
    LINK_TO,
    STABLE
};

class LinkEffect : public Node
{
public:
    static LinkEffect* create(Node* from, Node* to, float speed);
    LinkEffect();
    ~LinkEffect();
    virtual void update(float dt) override;
private:
    
    bool init(Node* from, Node* to, float speed);
    
    Node* mFrom;
    Node* mTo;
    ParticleSystemQuad* mLinkParticle;
    LINK_EFFECT_STATE mState;
    float mLinkSpeed;
    float mLinkTime;
};

NS_EE_END


#endif /* LinkEffect_hpp */
