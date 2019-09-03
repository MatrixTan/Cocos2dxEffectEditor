//
//  MainLayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/6/30.
//
//

#include "MainLayer.hpp"
#include "editor-support/cocostudio/CocoStudio.h"
#include "EngineEx/ShaderSprite.hpp"
#include "Effect/PostRenderEffectLayer.hpp"
#include "UI/UILayer.hpp"
#include "DrawLineLayer.hpp"
#include "Timeline/ActionDrawBezierPath.hpp"
#include "Project/Project.hpp"
#include "EngineEx/ParticleSystemExt.hpp"
#include "Common/Utils.hpp"

NS_EE_BEGIN

MainLayer::MainLayer()
:mCurrentSprite(nullptr)
{
}

MainLayer::~MainLayer()
{

}

bool MainLayer::init()
{
    
    FileUtils::getInstance()->addSearchPath("res/");
    
    /*auto test = CSLoader::createNode("res/horizontal_dispose.csb");
    cocostudio::timeline::ActionTimeline *timeline = CSLoader::createTimeline("res/horizontal_dispose.csb");
    test->runAction(timeline);
    timeline->gotoFrameAndPlay(0);
    timeline->addFrameEndCallFunc(40, "timeend", [test](){
        //test->removeFromParent();
    });
    this->addChild(test, 999999);
    test->setPosition(Vec2(300, 300));
    
    SpriteFrameCache::getInstance()->addSpriteFramesWithFile("res/item/candy_patch.plist");
    for(int i=0; i<30; i++){
        auto *sprite = Sprite::create();
        sprite->initWithSpriteFrameName(StringUtils::format("item_%d.png", i % 8 + 1));
        sprite->setPosition(cocos2d::random<float>(-300, 300), cocos2d::random<float>(-300.0f, 300.0f));
        auto move = MoveBy::create(3.0f, Vec2(cocos2d::random<float>(-200, 200), cocos2d::random<float>(-200.0f, 200.0f)));
        sprite->runAction(Repeat::create(Sequence::create(move, move->reverse(),NULL), INT_MAX));
        mContainer->getChildByName("root_node")->addChild(sprite);
    }*/
    
    
    
    auto touchEventListener = EventListenerTouchOneByOne::create();
    touchEventListener->setSwallowTouches(true);
    touchEventListener->onTouchBegan = CC_CALLBACK_2(MainLayer::onTouchBegin, this);
    touchEventListener->onTouchMoved = CC_CALLBACK_2(MainLayer::onTouchMove, this);
    touchEventListener->onTouchEnded = CC_CALLBACK_2(MainLayer::onTouchEnd, this);
    touchEventListener->onTouchCancelled = CC_CALLBACK_2(MainLayer::onTouchCancel, this);
    getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchEventListener, this);
    
    mRootNode = Node::create();
    addChild(mRootNode);
    mRootNode->setPosition(Director::getInstance()->getWinSize() * 0.5f);
    
    return Layer::init();
}


void MainLayer::setBackground(const std::string &file, const Vec2& scale)
{
    auto background = Sprite::create(file);
    background->setScale(scale.x, scale.y);
    mRootNode->addChild(background);
}

void MainLayer::addSprite(const std::string& id, ShaderSprite* pSprite, int zorder)
{
    mRootNode->addChild(pSprite, (int)SPRITE_ZORDER::SPRITE + zorder);
    pSprite->retain();
    mSprites.insert(std::pair<std::string, ShaderSprite*>(id, pSprite));
}

void MainLayer::addSprite(const std::string &id, ShaderSprite *pSprite, const std::string &maskId)
{
    auto iter = mMasks.find(maskId);
    if(iter != mMasks.end()){
        auto worldPos = pSprite->getPosition();
        iter->second->addChild(pSprite);
        auto maskWordPos = iter->second->getPosition();
        pSprite->setPosition(worldPos - maskWordPos);
        pSprite->retain();
        mSprites[id] = pSprite;
    }
}

ShaderSprite* MainLayer::getSprite(const std::string& id)
{
    std::map<std::string, ShaderSprite*>::iterator iter = mSprites.find(id);
    if(iter != mSprites.end())
    {
        return iter->second;
    }
    return nullptr;
}

void MainLayer::addParticleSystem(const std::string &id, cocos2d::ParticleSystemQuad *particle, int zorder)
{
    mRootNode->addChild(particle, (int)SPRITE_ZORDER::SPRITE + zorder);
    particle->retain();
    mParticles.insert(std::pair<std::string, ParticleSystemQuad*>(id, particle));
}

ParticleSystemQuad* MainLayer::getParticle(const std::string &id)
{
    std::map<std::string, ParticleSystemQuad*>::iterator iter = mParticles.find(id);
    if(iter != mParticles.end())
    {
        return iter->second;
    }
    return nullptr;
}

void MainLayer::addMask(const std::string& id, ClippingNode* node)
{
    mRootNode->addChild(node, (int)SPRITE_ZORDER::MASK);
    node->retain();
    mMasks[id] = node;
}

bool MainLayer::onTouchBegin(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(UILayer::getInstance()->getState() == UI_STATE::PEN){
        return DrawLineLayer::getInstance()->onTouchBegin(touch, event);
    }
    
    return true;
}

void MainLayer::onTouchMove(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(UILayer::getInstance()->getState() == UI_STATE::PEN){
        DrawLineLayer::getInstance()->onTouchMove(touch, event);
    }
}

void MainLayer::onTouchEnd(cocos2d::Touch *touch, cocos2d::Event *event)
{
    if(UILayer::getInstance()->getState() == UI_STATE::PEN){
        return DrawLineLayer::getInstance()->onTouchEnd(touch, event);
    }
    auto pos = touch->getLocation();
    ShaderSpriteMap::iterator iter = mSprites.begin();
    for(; iter != mSprites.end(); iter++){
        ShaderSprite* sprite = iter->second;
        auto size = sprite->getContentSize();
        Vec2 spritePos = sprite->convertToWorldSpace(Vec2::ZERO);
        auto r = sprite->getBoundingBox();
        if(r.containsPoint(pos)){
            setCurrentSprite(sprite);
        }
    }
    
    
    //PostRenderEffectLayer::getInstance()->setDrawRect(Rect(pos.x - 300, pos.y - 300, 600, 600), 0.70f);
}

void MainLayer::onTouchCancel(Touch *touch, Event *event)
{
    if(UILayer::getInstance()->getState() == UI_STATE::PEN){
        return DrawLineLayer::getInstance()->onTouchCancel(touch, event);
    }
}

void MainLayer::setCurrentSprite(ShaderSprite *sprite){
    if(mCurrentSprite != nullptr && mCurrentSprite != sprite){
        mCurrentSprite->enableSelect(false);
    }
    mCurrentSprite = sprite;
    mCurrentSprite->enableSelect(true);
}

void MainLayer::clear(){
    for(auto eachNode : mSprites){
        eachNode.second->release();
    }
    mSprites.clear();
    for(auto eachNode : mMasks){
        eachNode.second->release();
    }
    mMasks.clear();
    for(auto eachNode : mParticles){
        eachNode.second->release();
    }
    mParticles.clear();
    mRootNode->removeAllChildren();
}

void MainLayer::loadProject(Project *project){
    if(project == nullptr){
        return;
    }
    ProjectConfig* config = project->getConfig();
    setBackground(config->projectPath + config->background.file, config->background.scale);
    for(std::vector<std::string>::iterator iter = config->atlas.begin(); iter != config->atlas.end(); iter++)
    {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(config->projectPath + *iter);
    }
    
    for(std::map<std::string, MaskConfig*>::iterator iter = config->masks.begin(); iter != config->masks.end(); iter++)
    {
        auto stentil = Sprite::create(config->projectPath + iter->second->stencil);
        stentil->setScale(iter->second->scale.x, iter->second->scale.y);
        stentil->setPosition(Vec2::ZERO);
        auto clipNode = ClippingNode::create(stentil);
        
        clipNode->setAlphaThreshold(iter->second->alphaThreshold);
        clipNode->setPosition(iter->second->offset);
        addMask(iter->second->id, clipNode);
    }
    
    for(auto spriteIter : config->sprites)
    {
        auto shaderSprite = ShaderSprite::create();
        shaderSprite->initConfig(spriteIter.second);
        
        if(spriteIter.second->timeline.length() > 0){
            shaderSprite->runAction(config->timelines[spriteIter.second->timeline]->getAction());
        }
        
        if(spriteIter.second->mask.length() > 0){
            addSprite(spriteIter.second->id, shaderSprite, spriteIter.second->mask);
        }else{
            addSprite(spriteIter.second->id, shaderSprite, shaderSprite->getLocalZOrder());
        }
        
    }
    
    for(auto particleIter : config->particles){
        auto particle = ParticleSystemExt::create(config->projectPath + particleIter.second->file);
        particle->setPosition(particleIter.second->position.x, particleIter.second->position.y);
		particle->setScaleX(particleIter.second->scale.x);
		particle->setScaleY(particleIter.second->scale.y);
        particle->setRadial(particleIter.second->radial);
        if(particleIter.second->frameTile){
            particle->setFrameTile(particleIter.second->tileX, particleIter.second->tileY, particleIter.second->frameInterval);
        }
        if(particleIter.second->timeline.length() > 0){
            particle->runAction(config->timelines[particleIter.second->timeline]->getAction());
        }
        if(particleIter.second->randomTile){
            particle->setRandomFrame(particleIter.second->tileX, particleIter.second->tileY);
        }
        addParticleSystem(particleIter.second->id, particle, particleIter.second->position.z);
    }
    
    for(std::map<std::string, AnimationConfig*>::iterator iter = config->animations.begin();
        iter != config->animations.end();
        iter++)
    {
        SpriteFrameCache::getInstance()->addSpriteFramesWithFile(config->projectPath + iter->second->frameFile
                                                                 , config->projectPath + iter->second->texture);
        int frameFrom = iter->second->frameFrom;
        int frameTo = iter->second->frameTo;
        cocos2d::Vector<SpriteFrame*> frames;
        for(int i=frameFrom; i<=frameTo; i++){
            std::string frameName = Utils::stringFormat(iter->second->frameName.c_str(), 256, i);
            auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(frameName);
            frames.pushBack(spriteFrame);
        }
        auto animations = Animation::createWithSpriteFrames(frames, iter->second->interval*2);
        auto animator = Animate::create(animations);
        auto sprite = ShaderSprite::create();
        sprite->setPosition(iter->second->pos.x, iter->second->pos.y);
        sprite->setScale(iter->second->scale.x, iter->second->scale.y);
        sprite->setRotation(iter->second->rotation);
        sprite->setBlendFunc(BlendFunc::ADDITIVE);
        sprite->runAction(Sequence::create(DelayTime::create(iter->second->delay), Repeat::create(animator, iter->second->repeat), NULL));
        sprite->runAction(animator);
        sprite->setVisible(iter->second->visible);
        if(iter->second->timeline.length() > 0){
            sprite->runAction(config->timelines[iter->second->timeline]->getAction());
        }
        
        addSprite(iter->first, sprite, iter->second->pos.z);
    }
}

NS_EE_END
