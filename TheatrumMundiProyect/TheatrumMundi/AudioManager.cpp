#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include "Game.h"

// Sound class methods
Sound::Sound(const std::string& filePath) {
    source = AudioManager::Instance().createSource();
    buffer = AudioManager::Instance().loadWAV(filePath);
}

Sound::~Sound()
{
}

ALuint Sound::getSource() const
{
    return source;
}

ALuint Sound::getBuffer() const
{
    return buffer;
}


// AudioManager class methods
AudioManager& AudioManager::Instance() {
    static AudioManager instance; 
    return instance;
}

AudioManager::AudioManager() {
    // Get the list of available devices (for debug purposes)
   const ALCchar* deviceList = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    if (deviceList) {
#ifdef _DEBUG
        std::cout << "Dispositivos de audio disponibles:" << std::endl;
        while (*deviceList != '\0') {
            std::cout << " - " << deviceList << std::endl;
            deviceList += strlen(deviceList) + 1;
        }
#endif
    }


    // Try to open the default audio device
    const ALCchar* defaultDevice = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    device = alcOpenDevice(defaultDevice);
    if (!device) {
    #ifdef _DEBUG
        std::cerr << "Error: No se pudo abrir el dispositivo de audio: " << defaultDevice << std::endl;
        const ALCchar* error = alcGetString(nullptr, alcGetError(nullptr));
    #endif
    #ifdef _DEBUG
        std::cerr << "Razón: " << (error ? error : "Desconocido") << std::endl;
    #endif
    }

    context = alcCreateContext(device, nullptr); // Create a context
    if (!context) {
    #ifdef _DEBUG
        std::cerr << "Error: No se pudo crear el contexto de audio. Razón: ";
        const ALCchar* error = alcGetString(device, alcGetError(device));
    #endif
    #ifdef _DEBUG
        std::cerr << (error ? error : "Desconocido") << std::endl;
        alcCloseDevice(device);
    #endif

    }

    if (!alcMakeContextCurrent(context)) { // Make the current context
        #ifdef _DEBUG
        std::cerr << "Error: No se pudo hacer el contexto actual. Razón: ";
        const ALCchar* error = alcGetString(device, alcGetError(device));
        #endif
        #ifdef _DEBUG
        std::cerr << (error ? error : "Desconocido") << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);
        #endif

    }
    #ifdef _DEBUG
    std::cout << "OpenAL inicializado correctamente con el dispositivo: " << defaultDevice << std::endl;
    #endif
}


AudioManager::~AudioManager() {
    for (ALuint source : sources) {
        alDeleteSources(1, &source);
    }
    for (ALuint buffer : buffers) {
        alDeleteBuffers(1, &buffer);
    }

    alcDestroyContext(context); // Destroy the context
    alcCloseDevice(device);     // Close the device
}


// Load a WAV file into a buffer
ALuint 
AudioManager::loadWAV(const std::string& filePath) {
    SF_INFO sfInfo;
    SNDFILE* file = sf_open(filePath.c_str(), SFM_READ, &sfInfo);
    if (!file) {
        std::cerr << "No se pudo abrir el archivo: " << filePath << std::endl;
        return false;
    }

    std::vector<short> audioData(sfInfo.frames * sfInfo.channels);
    sf_read_short(file, audioData.data(), sfInfo.frames * sfInfo.channels);
    sf_close(file);

    ALenum formatAL;
    if (sfInfo.channels == 1) {
        formatAL = AL_FORMAT_MONO16;
    }
    else if (sfInfo.channels == 2) {
        formatAL = AL_FORMAT_STEREO16;
    }
    else {
        std::cerr << "Formato incorrecto: " << filePath << std::endl;
        return false;
    }
    ALuint buffer;
    alGenBuffers(1, &buffer);
    alBufferData(buffer, formatAL, audioData.data(), audioData.size() * sizeof(short), sfInfo.samplerate);

    buffers.push_back(buffer);

    return buffer;
}

// Create an audio source
ALuint 
AudioManager::createSource() {
    ALuint source;
    alGenSources(1, &source);
    sources.push_back(source);
    return source;
}

//Move the sound source position in the scene
void 
AudioManager::setSourcePosition(Sound sound, float x, float y, float z) {
    alSource3f(sound.getSource(), AL_POSITION, x, y, z);
}

//Move the listener position in the scene
void 
AudioManager::setListenerPosition(float x, float y, float z) {
    alListener3f(AL_POSITION, x, y, z);
}


//Modify the track volume
void 
AudioManager::setVolume(Sound sound, ALfloat volume) {
    alSourcef(sound.getSource(), AL_GAIN, volume);
}

//Modify the track speed
void AudioManager::setSpeed(Sound sound, ALfloat speed) {
    alSourcef(sound.getSource(), AL_VELOCITY, speed);
}

//Modify the track pitch
void AudioManager::setPitch(Sound sound, ALfloat pitch) {
    alSourcef(sound.getSource(), AL_PITCH, pitch);
}

// Play a sound (default, once)
void 
AudioManager::playSound(Sound sound) {
    alSourcei(sound.getSource(), AL_BUFFER, sound.getBuffer());
    alSourcePlay(sound.getSource());
}

// Stop a sound
void 
AudioManager::stopSound(Sound sound) {
    alSourceStop(sound.getSource());
}

// Pause a sound
void 
AudioManager::pauseSound(Sound sound) {
    alSourcePause(sound.getSource());
}

// Resume a sound
void
AudioManager::resumeSound(Sound sound) {
    alSourcePlay(sound.getSource());
}

// Set a sound in/out a loop
void AudioManager::setLooping(Sound sound,bool loop)
{
    alSourcei(sound.getSource(), AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}