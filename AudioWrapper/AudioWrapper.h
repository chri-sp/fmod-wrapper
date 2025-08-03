#pragma once
#include <string>
#include <map>
#include "fmod.hpp"

class AudioSystem
{
public:
    AudioSystem();
    ~AudioSystem();

    bool init();
    void shutdown();

    bool loadSound(const std::string& name, const std::string& path);
    void playSound(const std::string& name);

    void pauseSound(const std::string& name, bool pause);
    void stopSound(const std::string& name);


private:
    FMOD::System* mSystem = nullptr;
    std::map<std::string, FMOD::Sound*> mSounds;
    std::map<std::string, FMOD::Channel*> mChannels;
};
