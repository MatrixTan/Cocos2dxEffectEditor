//
//  MainLayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#ifndef MainLayer_hpp
#define MainLayer_hpp

#include <stdio.h>
#include "EffectEditor.hpp"
#include "GlobalDefine.hpp"
#include "Common/NodeSingleton.hpp"
#include "ShaderSprite.hpp"
#include "Project/MaskConfig.hpp"
#include "Project/Project.hpp"

NS_EE_BEGIN

using namespace cocos2d;

class MainLayer : public NodeSingleton<MainLayer>, public Layer
{
public:
    MainLayer();
    ~MainLayer();
    virtual bool init(void);
    
    void addSprite(const std::string& id, ShaderSprite *pSprite, int zorder = 0);
    void addSprite(const std::string& id, ShaderSprite *pSprite, const std::string& maskId);
    ShaderSprite* getSprite(const std::string& id);
    void setCurrentSprite(ShaderSprite* sprite);
    
    void addParticleSystem(const std::string& id, ParticleSystemQuad* particle, int zorder = 0);
    ParticleSystemQuad* getParticle(const std::string& id);
    
    void addMask(const std::string& id, ClippingNode* node);
    
    void setBackground(const std::string& file, const Vec2& scale);
    void loadProject(Project* project);
    void clear();
    CREATE_FUNC(MainLayer);
private:
    
    bool onTouchBegin(Touch *touch, Event *event);
    void onTouchMove(Touch *touch, Event *event);
    void onTouchEnd(Touch *touch, Event *event);
    void onTouchCancel(Touch *touch, Event *event);
    
    typedef std::map<std::string, ShaderSprite*> ShaderSpriteMap;
    ShaderSpriteMap mSprites;
    std::map<std::string, ParticleSystemQuad*> mParticles;
    std::map<std::string, ClippingNode*> mMasks;
    
    ShaderSprite* mCurrentSprite = nullptr;
    Node* mRootNode = nullptr;
};

NS_EE_END

#endif /* MainLayer_hpp */
