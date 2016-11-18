//
//  PlatformAdapter.cpp
//  EffectEditor
//
//  Created by Matrix on 16/10/14.
//
//

#include <stdio.h>
#include "PlatformAdapter.h"

#include <string>

USING_NS_CC;

NS_EE_BEGIN

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

#include "platform/android/jni/JniHelper.h"

#define ANDROID_ADAPTER_CLASS "org/cocos2dx/cpp/AndroidAdapter"

std::string callStaticStringMethod(const std::string& className, const std::string& functionName)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, className.c_str(), functionName.c_str(),"()Ljava/lang/String;");
    
    if (isHave)
    {
        jstring jstr=(jstring)minfo.env->CallStaticObjectMethod(minfo.classID,minfo.methodID);
        return JniHelper::jstring2string(jstr);
    }
    else
    {
        return "";
    }
}

void callStaticVoidMethod(const std::string& className, const std::string& functionName)
{
    JniMethodInfo minfo;
    bool isHave = JniHelper::getStaticMethodInfo(minfo, className.c_str(), functionName.c_str(),"()V");
    
    if (isHave)
    {
        minfo.env->CallStaticVoidMethod(minfo.classID, minfo.methodID);
    }
}

bool PlatformAdapter::isOtherAudioPlaying(void)
{
    std::string ret = callStaticStringMethod(ANDROID_ADAPTER_CLASS, "isOtherAudioPlaying");
    return ret == "true";
}

void PlatformAdapter::stopOtherAudio(void)
{
    callStaticVoidMethod(ANDROID_ADAPTER_CLASS, "requestAudioFocus");
}

void PlatformAdapter::resumeOtherAudio(void)
{
    callStaticVoidMethod(ANDROID_ADAPTER_CLASS, "abandonAudioFocus");
}

std::string PlatformAdapter::getFilePath(const std::string &types)
{
    return "";
}

std::string PlatformAdapter::getSaveFilePath(const std::string &types)
{
    return "";
}

#endif

NS_EE_END