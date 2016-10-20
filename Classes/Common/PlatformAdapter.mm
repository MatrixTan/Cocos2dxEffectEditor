//
//  PlatformAdapter.m
//  EffectEditor
//
//  Created by Matrix on 16/10/13.
//
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#include "PlatformAdapter.h"

NS_EE_BEGIN

bool PlatformAdapter::isOtherAudioPlaying(void)
{
#if (CC_TARGET_PLATFORM == CC_TARGET_IOS)
    return [[AVAudioSession sharedInstance] isOtherAudioPlaying];
#endif
    return false;
}

void PlatformAdapter::stopOtherAudio()
{
}

void PlatformAdapter::resumeOtherAudio()
{
}


NS_EE_END