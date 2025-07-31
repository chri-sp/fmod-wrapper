// AudioWrapper.cpp : Defines the functions for the static library.

#include "pch.h"
#include "framework.h"
#include "AudioWrapper.h"
#include <iostream>

AudioSystem::AudioSystem() {}

AudioSystem::~AudioSystem() {
    shutdown();
}

bool AudioSystem::init() {
    if (FMOD::System_Create(&mSystem) != FMOD_OK) {
        std::cerr << "FMOD system creation failed.\n";
        return false;
    }
    if (mSystem->init(512, FMOD_INIT_NORMAL, nullptr) != FMOD_OK) {
        std::cerr << "FMOD system initialization failed.\n";
        return false;
    }
    return true;
}

void AudioSystem::shutdown() {
    for (auto it = mSounds.begin(); it != mSounds.end(); ++it) {
        if (it->second) {
            it->second->release();
        }
    }

    mSounds.clear();
    mChannels.clear();

    if (mSystem) {
        mSystem->close();
        mSystem->release();
        mSystem = nullptr;
    }
}

bool AudioSystem::loadSound(const std::string& name, const std::string& path) {
    FMOD::Sound* sound = nullptr;
    if (mSystem->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound) != FMOD_OK) {
        std::cerr << "Unable to load: " << path << "\n";
        return false;
    }

    mSounds[name] = sound;
    return true;
}

void AudioSystem::playSound(const std::string& name) {
    auto it = mSounds.find(name);
    if (it != mSounds.end()) {
        FMOD::Channel* channel = nullptr;
        if (mSystem->playSound(it->second, nullptr, false, &channel) == FMOD_OK) {
            mChannels[name] = channel;
        }
        else {
            std::cerr << "Unable to play: " << name << "\n";
        }
    }
    else {
        std::cerr << "Sound not found: " << name << "\n";
    }
}
