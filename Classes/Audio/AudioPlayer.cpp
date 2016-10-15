//
//  AudioPlayer.cpp
//  EffectEditor
//
//  Created by Matrix on 16/10/10.
//
//

#include "AudioPlayer.hpp"
#include "SimpleAudioEngine.h"
#include "AudioEngine.h"
#include "PlatformAdapter.h"

NS_EE_BEGIN

USING_NS_CC;
using namespace CocosDenshion;

#define AUDIO_ENABLE_MUSIC_KEY "AUDIO_ENABLE_MUSIC_KEY"
#define AUDIO_ENABLE_SOUND_KEY "AUDIO_ENABLE_SOUND_KEY"
#define AUDIO_ENABLE_KEY_NONE -1
#define AUDIO_ENABLE_KEY_ENABLE 1
#define AUDIO_ENABLE_KEY_DISABLE 0

bool AudioPlayer::init()
{
    int enableMusicKey = UserDefault::getInstance()->getIntegerForKey(AUDIO_ENABLE_MUSIC_KEY, AUDIO_ENABLE_KEY_NONE);
    if(enableMusicKey == AUDIO_ENABLE_KEY_NONE){
        UserDefault::getInstance()->setIntegerForKey(AUDIO_ENABLE_MUSIC_KEY, AUDIO_ENABLE_KEY_ENABLE);
        enableMusicKey = AUDIO_ENABLE_KEY_ENABLE;
        UserDefault::getInstance()->flush();
    }
    mEnableMusic = enableMusicKey == AUDIO_ENABLE_KEY_ENABLE;
    
    int enableSoundKey = UserDefault::getInstance()->getIntegerForKey(AUDIO_ENABLE_SOUND_KEY, AUDIO_ENABLE_KEY_NONE);
    if(enableSoundKey == AUDIO_ENABLE_KEY_NONE){
        UserDefault::getInstance()->setIntegerForKey(AUDIO_ENABLE_SOUND_KEY, AUDIO_ENABLE_KEY_ENABLE);
        enableSoundKey = AUDIO_ENABLE_KEY_ENABLE;
        UserDefault::getInstance()->flush();
    }
    mEnableSound = enableSoundKey == AUDIO_ENABLE_KEY_ENABLE;
    for(int i=0; i<MAX_MUSIC_NUM; i++){
        mMusic[i] = nullptr;
    }
    return true;
}

void AudioPlayer::setMusicEnable(bool enable)
{
    mEnableMusic = enable;
    UserDefault::getInstance()->setIntegerForKey(AUDIO_ENABLE_MUSIC_KEY, enable ? AUDIO_ENABLE_KEY_ENABLE: AUDIO_ENABLE_KEY_DISABLE);
    UserDefault::getInstance()->flush();
}

void AudioPlayer::setSoundEnable(bool enable)
{
    mEnableSound = enable;
    UserDefault::getInstance()->setIntegerForKey(AUDIO_ENABLE_SOUND_KEY, enable ? AUDIO_ENABLE_KEY_ENABLE : AUDIO_ENABLE_KEY_DISABLE);
    UserDefault::getInstance()->flush();
}

bool AudioPlayer::isMusicEnabled()
{
    return mEnableMusic;
}

bool AudioPlayer::isSoundEnabled()
{
    return mEnableSound;
}

void AudioPlayer::playMusic(const std::string &musicPath, bool loop, int channel)
{
    if(mEnableMusic && channel >=0 && channel < MAX_MUSIC_NUM){
        int audioId = experimental::AudioEngine::play2d(musicPath.c_str(), loop);
        MusicPlayTask* task = new MusicPlayTask();
        task->audioId = audioId;
        task->audioName = musicPath;
        task->loop = loop;
        stopMusic(channel);
        mMusic[channel] = task;
        
        auto onMusicComplete = [&, channel](int audioId, const std::string &info)
        {
            if(mMusic[channel] != nullptr){
                delete mMusic[channel];
                mMusic[channel] = nullptr;
            }
        };
        
        experimental::AudioEngine::setFinishCallback(audioId, onMusicComplete);
    }
}

void AudioPlayer::pauseMusic(int channel)
{
    if(channel >=0 && channel <MAX_MUSIC_NUM){
        if(mMusic[channel] != nullptr){
            experimental::AudioEngine::pause(mMusic[channel]->audioId);
        }
    }
}

void AudioPlayer::resumeMusic(int channel)
{
    if(channel >=0 && channel <MAX_MUSIC_NUM){
        if(mMusic[channel] != nullptr){
            experimental::AudioEngine::resume(mMusic[channel]->audioId);
        }
    }
}


void AudioPlayer::stopMusic(int channel)
{
    if(channel >= 0 && channel < MAX_MUSIC_NUM)
    {
        if(mMusic[channel] != nullptr){
            experimental::AudioEngine::stop(mMusic[channel]->audioId);
            delete mMusic[channel];
            mMusic[channel] = nullptr;
        }
    }
}

void AudioPlayer::playSound(const std::string &soundPath, bool loop)
{
    if(mEnableSound){
        int audioId = experimental::AudioEngine::play2d(soundPath.c_str(), loop);
        if(mPlayingSounds.find(soundPath) != mPlayingSounds.end()){
            mPlayingSounds[soundPath]->audioIds.push_back(audioId);
        }else{
            auto soundTask = new SoundPlayTask();
            soundTask->audioName = soundPath;
            soundTask->audioIds.push_back(audioId);
            mPlayingSounds[soundPath] = soundTask;
        }
        
        auto onSoundComplete = [&](int audio, const std::string& info)
        {
            std::map<std::string, SoundPlayTask*>::iterator task = mPlayingSounds.find(info);
            if(task != mPlayingSounds.end()){
                auto oneSoundTask = task->second;
                for(std::vector<int>::iterator iter = oneSoundTask->audioIds.begin();
                    iter != oneSoundTask->audioIds.end();){
                    if(*iter == audio){
                        oneSoundTask->audioIds.erase(iter);
                        continue;
                    }else{
                        iter++;
                    }
                }
            }
        };
        experimental::AudioEngine::setFinishCallback(audioId, onSoundComplete);
    }
}

void AudioPlayer::playSound(const std::string &soundPath, int max, bool loop)
{
    if(mEnableSound){
        std::map<std::string, SoundPlayTask*>::iterator task = mPlayingSounds.find(soundPath);
        if(task != mPlayingSounds.end()){
            if(task->second->audioIds.size() >= max){
                return;
            }
        }
        playSound(soundPath, loop);
    }
}

void AudioPlayer::stopSound(const std::string &soundPath)
{
    std::map<std::string, SoundPlayTask*>::iterator task = mPlayingSounds.find(soundPath);
    if(task != mPlayingSounds.end()){
        for(std::vector<int>::iterator iter = task->second->audioIds.begin();
            iter != task->second->audioIds.end(); iter++){
            experimental::AudioEngine::stop(*iter);
        }
        task->second->audioIds.clear();
    }
}

bool AudioPlayer::isOtherAudioPlaying()
{
    return PlatformAdapter::isOtherAudioPlaying();
}

void AudioPlayer::stopOtherAudio()
{
    PlatformAdapter::stopOtherAudio();
}

void AudioPlayer::resumeOtherAudio()
{
    PlatformAdapter::resumeOtherAudio();
}

void AudioPlayer::stopAll()
{
    for(int i=0; i<MAX_MUSIC_NUM; i++){
        stopMusic(i);
    }
    
    std::map<std::string, SoundPlayTask*>::iterator task = mPlayingSounds.begin();
    for(; task != mPlayingSounds.end(); task++){
        for(std::vector<int>::iterator iter = task->second->audioIds.begin();
            iter != task->second->audioIds.end(); iter++){
            experimental::AudioEngine::stop(*iter);
        }
    }
    mPlayingSounds.clear();
}

void AudioPlayer::pauseAll()
{
    experimental::AudioEngine::pauseAll();
}

void AudioPlayer::resumeAll()
{
    experimental::AudioEngine::resumeAll();
}

NS_EE_END
