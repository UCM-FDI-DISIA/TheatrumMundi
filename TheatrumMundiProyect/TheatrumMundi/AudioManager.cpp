#include "AudioManager.h"
#include "../lib/FMOD/include/fmod_errors.h"
#include <iostream>


//Class Sound
Sound::Sound(const std::string& filePath) {
    sound = nullptr;
    channel = nullptr;
}

Sound::~Sound() {
    if (sound != nullptr) {
        sound->release();
    }
}

FMOD::Sound* Sound::getSound() const { return sound; }

void Sound::setSound(FMOD::Sound* s)
{
    sound = s;
}

FMOD::Channel* Sound::getChannel() const { return channel; }
void Sound::setChannel(FMOD::Channel* ch) { channel = ch; }



//AUDIO MANAGER
/*
AudioManager& AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}
*/
bool AudioManager::init() {
    FMOD_RESULT result;
    result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
#ifdef DEBUG
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
#endif // DEBUG
        return false;
    }

    result = system->init(128, FMOD_INIT_NORMAL, nullptr);
    if (result != FMOD_OK) {
#ifdef DEBUG
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
#endif
        return false;
    }

    return true;
}

void AudioManager::shutdown() {
    sounds.clear();  
    if (system) {
        system->close();
        system->release();
    }
}

std::shared_ptr<Sound> AudioManager::createSound(const std::string& filePath) {
    auto soundPtr = std::make_shared<Sound>(filePath);
    Sound* rawSound = soundPtr.get();

    FMOD::Sound* fmodSound = nullptr;

    FMOD_RESULT result = system->createSound(
        filePath.c_str(), FMOD_DEFAULT, nullptr, &fmodSound
    );

    if (result != FMOD_OK) {
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
        return nullptr;
    }

    soundPtr->setSound(fmodSound);
    sounds.push_back(soundPtr);
    return soundPtr;
}

void AudioManager::playSound(std::shared_ptr<Sound> sound, bool loop) {
    if (!sound || !sound->getSound()) return;

    // Check if is already playing
    FMOD::Channel* channel = sound->getChannel();
    bool isPlaying = false;
    if (channel) {
        channel->isPlaying(&isPlaying);
    }
    if (isPlaying) {
        return;
    }

    // Set loop
    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    sound->getSound()->setMode(mode);

    // Play new channel
    FMOD_RESULT result = system->playSound(
        sound->getSound(),
        nullptr,
        false,
        &channel
    );

    if (result == FMOD_OK) {
        sound->setChannel(channel);
    }
    else {
#ifdef DEBUG
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
#endif // DEBUG


    }

#ifdef DEBUG
    std::cout << "canales usados: " << getUsedChannels() << std::endl;
#endif // DEBUG


}


void AudioManager::stopSound(std::shared_ptr<Sound> sound) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->stop();
    }
}

void AudioManager::pauseSound(std::shared_ptr<Sound> sound) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setPaused(true);
    }
}

void AudioManager::resumeSound(std::shared_ptr<Sound> sound) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setPaused(false);
    }
}

int AudioManager::getUsedChannels() const {
    int usedChannels = 0;
    system->getChannelsPlaying(&usedChannels);
    return  usedChannels;
}

void AudioManager::cleanupInactiveChannels()
{
    if (!system) return;

    
    for (auto& soundPtr : sounds) {
        if (soundPtr && soundPtr->getChannel()) {
            bool isPlaying = false;
            FMOD_RESULT result = soundPtr->getChannel()->isPlaying(&isPlaying);

            
            if (result != FMOD_OK || !isPlaying) {
                soundPtr->setChannel(nullptr); // 
            }
        }
    }


    sounds.erase(
        std::remove_if(sounds.begin(), sounds.end(),
            [](const std::shared_ptr<Sound>& s) {
                return s.use_count() == 1;
            }),
        sounds.end()
    );
}

void AudioManager::set3DPosition(std::shared_ptr<Sound> sound, float x, float y, float z) {
    if (sound && sound->getChannel()) {
        FMOD_VECTOR pos = { x, y, z };
        sound->getChannel()->set3DAttributes(&pos, nullptr);
    }
}

void AudioManager::setListenerPosition(float x, float y, float z) {
    if (system) {
        FMOD_VECTOR pos = { x, y, z };
        FMOD_VECTOR vel = { 0, 0, 0 };
        FMOD_VECTOR forward = { 0, 0, 1 };
        FMOD_VECTOR up = { 0, 1, 0 };

        system->set3DListenerAttributes(
            0, // Listener id (0 is single-listener)
            &pos,
            &vel,
            &forward,
            &up
        );
    }
}

void AudioManager::setVolume(std::shared_ptr<Sound> sound, float volume) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setVolume(volume);
    }
}

void AudioManager::setPitch(std::shared_ptr<Sound> sound, float pitch) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setPitch(pitch);
    }
}

void AudioManager::update() {
    if (system) {
        cleanupInactiveChannels();
        system->update();
    }
}

AudioManager::AudioManager() {
    init();
}


AudioManager::~AudioManager()
{
    shutdown();
}
