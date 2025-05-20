#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../lib/FMOD/include/fmod.h"
#include "../lib/FMOD/include/fmod.hpp"
#include "../lib/FMOD/include/fmod_errors.h"
#include "../lib/FMOD/include/fmod_codec.h"
#include "../lib/FMOD/include/fmod_common.h"
#include "../lib/FMOD/include/fmod_dsp.h"
#include "../lib/FMOD/include/fmod_dsp_effects.h"
#include "../lib/FMOD/include/fmod_output.h"
#include "Singleton.h"

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

class AudioManager : public Singleton<AudioManager> {
    friend Singleton<AudioManager>;
public:
    // Singleton
 //   static AudioManager& Instance();
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    // Initialization and shutdown
    bool init();
    void shutdown();

    // Load and play
    std::shared_ptr<Sound> createSound(const std::string& filePath);
    void playSound(std::shared_ptr<Sound> sound, bool loop = false);
    void stopSound(std::shared_ptr<Sound> sound);
    void pauseSound(std::shared_ptr<Sound> sound);
    void resumeSound(std::shared_ptr<Sound> sound);

    void cleanupInactiveChannels();


    // Setters
    void setVolume(std::shared_ptr<Sound> sound, float volume);
    void setPitch(std::shared_ptr<Sound> sound, float pitch);
    void set3DPosition(std::shared_ptr<Sound> sound, float x, float y, float z);
    void setListenerPosition(float x, float y, float z);

    void update();

private:

    int getUsedChannels() const;

    AudioManager();
    ~AudioManager();

    FMOD::System* system = nullptr;
    std::vector<std::shared_ptr<Sound>> sounds;
};


inline AudioManager& audioManager() {
    return *AudioManager::Instance();
}