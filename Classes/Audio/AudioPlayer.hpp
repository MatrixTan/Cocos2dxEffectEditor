//
//  AudioPlayer.hpp
//  EffectEditor
//
//  Created by Matrix on 16/10/10.
//
//

#ifndef AudioPlayer_hpp
#define AudioPlayer_hpp

#include <stdio.h>
#include "GlobalDefine.hpp"
#include "Common/Singleton.hpp"
#include <cocos2d.h>
#include <map>
#include <vector>

NS_EE_BEGIN

#define MAX_MUSIC_NUM 8

class MusicPlayTask
{
public:
    int audioId;
    std::string audioName;
    bool loop;
};

class SoundPlayTask
{
public:
    std::string audioName;
    std::vector<int> audioIds;
};

class AudioPlayer : public Singleton<AudioPlayer>
{
public:
    bool init(void);
    void setMusicEnable(bool enable);
    void setSoundEnable(bool enable);
    bool isMusicEnabled(void);
    bool isSoundEnabled(void);
    
    void playMusic(const std::string& musicPath, bool loop = true, int channel = 0);
    void stopMusic(int channel = 0);
    void pauseMusic(int channel = 0);
    void resumeMusic(int channel = 0);
    
    void playSound(const std::string& soundPath, bool loop = false);
    void playSound(const std::string& soundPath, int max, bool loop = false);
    void stopSound(const std::string& soundPath);
    
    bool isOtherAudioPlaying(void);
    void stopOtherAudio(void);
    void resumeOtherAudio(void);
    void stopAll(void);
    void pauseAll(void);
    void resumeAll(void);
    
private:
    bool mEnableMusic;
    bool mEnableSound;
    std::map<std::string, SoundPlayTask*> mPlayingSounds;
    MusicPlayTask* mMusic[MAX_MUSIC_NUM];
};


NS_EE_END

#endif /* AudioPlayer_hpp */
