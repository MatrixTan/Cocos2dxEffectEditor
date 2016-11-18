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
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
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

std::string PlatformAdapter::getFilePath(const std::string &types)
{
    std::string ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    NSString *str = [NSString stringWithUTF8String:types.c_str()];
    NSArray *fileTypes = [NSArray arrayWithObjects:str, nil];
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setAllowsMultipleSelection:NO];
    [panel setCanChooseDirectories:NO];
    [panel setCanChooseFiles:YES];
    [panel setFloatingPanel:YES];
    NSInteger result = [panel runModalForDirectory:NSHomeDirectory()
                                        file:nil
                                           types:fileTypes];
    if(result == NSOKButton)
    {
        std::string url = [[[[panel URLs] objectAtIndex:0] absoluteString] UTF8String];
        if(url.length() > 7){
            ret = url.substr(7, url.length()-7);
        }
    }
    
#endif
    return ret;
}

std::string PlatformAdapter::getSaveFilePath(const std::string &type)
{
    std::string ret;
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    std::string defaultName = "name.";
    defaultName += type;
    NSString *str = [NSString stringWithUTF8String:type.c_str()];
    NSArray *fileTypes = [NSArray arrayWithObjects:str, nil];
    NSSavePanel *panel = [NSSavePanel savePanel];
    
    [panel setFloatingPanel:YES];
    [panel setNameFieldStringValue:[NSString stringWithUTF8String:defaultName.c_str()]];
    NSInteger result = [panel runModalForDirectory:NSHomeDirectory()
                                              file:nil
                                             types:fileTypes];
    if(result == NSOKButton)
    {
        std::string url = [[[panel URL] absoluteString] UTF8String];
        if(url.length() > 7){
            ret = url.substr(7, url.length()-7);
        }
    }
    
#endif
    return ret;
}


NS_EE_END