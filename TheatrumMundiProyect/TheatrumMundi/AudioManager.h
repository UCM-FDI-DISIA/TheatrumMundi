#pragma once
#include <string>
#include <vector>
#include <memory>
#include "fmod.h"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "fmod_codec.h"
#include "fmod_common.h"
#include "fmod_dsp.h"
#include "fmod_dsp_effects.h"
#include "fmod_output.h"

class Sound {
public:
    Sound(const std::string& filePath = "");
    ~Sound();

    FMOD::Sound* getSound() const;
    void setSound(FMOD::Sound* s);

    FMOD::Channel* getChannel() const;
    void setChannel(FMOD::Channel* channel);

private:
    FMOD::Sound* sound = nullptr;
    FMOD::Channel* channel = nullptr;
};

class AudioManager {
public:
    // Singleton
    static AudioManager& Instance();
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Initialization and shutdown
    bool init();
    void shutdown();

    // Load and play
    Sound* createSound(const std::string& filePath);
    void playSound(Sound* sound, bool loop = false);
    void stopSound(Sound* sound);
    void pauseSound(Sound* sound);
    void resumeSound(Sound* sound);

    // Setters
    void setVolume(Sound* sound, float volume);
    void setPitch(Sound* sound, float pitch);
    void set3DPosition(Sound* sound, float x, float y, float z);
    void setListenerPosition(float x, float y, float z);

    void update();

private:
    AudioManager() = default;
    ~AudioManager();

    FMOD::System* system = nullptr;
    std::vector<std::unique_ptr<Sound>> sounds;
};