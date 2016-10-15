//
//  PlatformAdapter.h
//  EffectEditor
//
//  Created by Matrix on 16/10/13.
//
//

#ifndef PlatformAdapter_h
#define PlatformAdapter_h
#include "GlobalDefine.hpp"

NS_EE_BEGIN


class PlatformAdapter
{
public:
    static bool isOtherAudioPlaying(void);
    static void stopOtherAudio(void);
    static void resumeOtherAudio(void);
};

NS_EE_END



#endif /* PlatformAdapter_h */

