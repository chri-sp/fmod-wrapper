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

bool AudioSystem::loadSound(const std::string& name, const std::string& path, bool loop, bool stream) {

    // Release existing sound with the same name
    auto existing = mSounds.find(name);
    if (existing != mSounds.end() && existing->second) {
        existing->second->release();
    }
    
    FMOD::Sound* sound = nullptr;

    FMOD_MODE mode = FMOD_DEFAULT;
    if (loop)
        mode |= FMOD_LOOP_NORMAL;
    else 
        mode |= FMOD_LOOP_OFF;

    if (stream)
        mode |= FMOD_CREATESTREAM;

    if (mSystem->createSound(path.c_str(), mode, nullptr, &sound) != FMOD_OK) {
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


void AudioSystem::pauseSound(const std::string& name, bool pause) {
    auto it = mChannels.find(name);
    if (it != mChannels.end() && it->second) {
        it->second->setPaused(pause);
    } else {
        std::cerr << "Cannot pause: " << name << "\n";
    }
}

void AudioSystem::stopSound(const std::string& name) {
    auto it = mChannels.find(name);
    if (it != mChannels.end() && it->second) {
        it->second->stop();
    } else {
        std::cerr << "Cannot stop: " << name << "\n";
    }
}

void AudioSystem::setPan(const std::string& name, float pan) {
    auto it = mChannels.find(name);
    if (it != mChannels.end() && it->second) {
        // Clamp pan between -1.0 (left) and 1.0 (right)
        pan = std::max(-1.0f, std::min(1.0f, pan));
        it->second->setPan(pan);
    } else {
        std::cerr << "Cannot set pan: " << name << "\n";
    }
}

void AudioSystem::setVolume(const std::string& name, float volume) {
    auto it = mChannels.find(name);
    if (it != mChannels.end() && it->second) {
        // Clamp volume between 0.0 (mute) and 1.0 (max)
        volume = std::max(0.0f, std::min(1.0f, volume));
        it->second->setVolume(volume);
    } else {
        std::cerr << "Cannot set volume: " << name << "\n";
    }
}

