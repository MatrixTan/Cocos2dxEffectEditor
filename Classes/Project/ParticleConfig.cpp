//
//  ParticleConfig.cpp
//  EffectEditor
//
//  Created by Matrix on 16/8/1.
//
//

#include "ParticleConfig.hpp"

NS_EE_BEGIN

ParticleConfig::ParticleConfig()
:radial(false),
frameTile(false),
frameInterval(0.0f),
tileX(1),
tileY(1),
randomTile(false)
{
    scale = cocos2d::Vec2::ONE;
}

NS_EE_END