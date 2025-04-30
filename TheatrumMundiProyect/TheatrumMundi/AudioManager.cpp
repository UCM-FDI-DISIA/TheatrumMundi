#include "AudioManager.h"
#include "fmod_errors.h"
#include <iostream>


//Class Sound
Sound::Sound(const std::string& filePath) {
    sound = nullptr;
    channel = nullptr;
}

Sound::~Sound() {
    if (sound) {
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
AudioManager& AudioManager::Instance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::init() {
    FMOD_RESULT result;
    result = FMOD::System_Create(&system);
    if (result != FMOD_OK) {
#ifdef DEBUG
        std::cerr << "FMOD error: " << FMOD_ErrorString(result) << std::endl;
#endif // DEBUG
        return false;
    }

    result = system->init(32, FMOD_INIT_NORMAL, nullptr);
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

Sound* AudioManager::createSound(const std::string& filePath) {
    // Create a unique_ptr and save the raw pointer
    auto soundPtr = std::make_unique<Sound>(filePath);
    Sound* rawSound = soundPtr.get();

    FMOD::Sound* fmodSound = nullptr;
    FMOD_RESULT result = system->createSound(
        filePath.c_str(), FMOD_DEFAULT, nullptr, &fmodSound
    );

    if (result != FMOD_OK) {
#ifdef DEBUG
        std::cerr << "FMOD error al cargar sonido: " << FMOD_ErrorString(result) << std::endl;
#endif
        return nullptr;
    }

    soundPtr->setSound(fmodSound);
    sounds.push_back(std::move(soundPtr));  // Transfer ownership to the vector
    return rawSound;  // And returns the raw pointer (for external use)
}

void AudioManager::playSound(Sound* sound, bool loop) {
    if (!sound || !sound->getSound()) return;

    FMOD_MODE mode = loop ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
    sound->getSound()->setMode(mode);

    FMOD::Channel* channel = nullptr;
    system->playSound(sound->getSound(), nullptr, false, &channel);
    sound->setChannel(channel);
}


void AudioManager::stopSound(Sound* sound) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->stop();
    }
}

void AudioManager::pauseSound(Sound* sound) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setPaused(true);
    }
}

void AudioManager::resumeSound(Sound* sound) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setPaused(false);
    }
}

void AudioManager::set3DPosition(Sound* sound, float x, float y, float z) {
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

void AudioManager::setVolume(Sound* sound, float volume) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setVolume(volume);
    }
}

void AudioManager::setPitch(Sound* sound, float pitch) {
    if (sound && sound->getChannel()) {
        sound->getChannel()->setPitch(pitch);
    }
}

void AudioManager::update() {
    if (system) {
        system->update();
    }
}

AudioManager::~AudioManager()
{
    shutdown();
}
