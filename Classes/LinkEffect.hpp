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

class LinkEffect : public Node
{
public:
    bool init(Node* from, Node* to);
    static LinkEffect* create(Node* from, Node* to);
    LinkEffect();
    ~LinkEffect();
    virtual void update(float dt) override;
private:
    Node* mFrom;
    Node* mTo;
    ParticleSystemQuad* mLinkParticle;
    
};

NS_EE_END


#endif /* LinkEffect_hpp */
