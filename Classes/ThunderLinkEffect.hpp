//
//  ThunderLinkEffect.hpp
//  EffectEditor
//
//  Created by Matrix on 16/8/9.
//
//

#ifndef ThunderLinkEffect_hpp
#define ThunderLinkEffect_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"

NS_EE_BEGIN

USING_NS_CC;


class ShaderSprite;

class ThunderLinkEffect : public Node
{
public:
    static ThunderLinkEffect* create(Node* from, Node* to, float speed);
    ThunderLinkEffect();
    ~ThunderLinkEffect();
    virtual void update(float dt) override;
private:    
    bool init(Node* from, Node* to, float speed);
    void updateThunder();
    
    Node* mFrom;
    Node* mTo;
    ShaderSprite* mSprite;
    float mLinkSpeed;
};

NS_EE_END

#endif /* ThunderLinkEffect_hpp */
