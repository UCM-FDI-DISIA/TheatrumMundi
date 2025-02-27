#include "AudioManager.h"
#include <iostream>
#include <fstream>
#include <cstdint>


AudioManager::AudioManager() {
    // Get the list of available devices (for debug purposes)
   const ALCchar* deviceList = alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    if (deviceList) {
        if (_DEBUG)
        std::cout << "Dispositivos de audio disponibles:" << std::endl;
        while (*deviceList != '\0') {
            std::cout << " - " << deviceList << std::endl;
            deviceList += strlen(deviceList) + 1;
        }
    }


    // Try to open the default audio device
    const ALCchar* defaultDevice = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    device = alcOpenDevice(defaultDevice);
    if (!device) {
        if (_DEBUG)
        std::cerr << "Error: No se pudo abrir el dispositivo de audio: " << defaultDevice << std::endl;
        const ALCchar* error = alcGetString(nullptr, alcGetError(nullptr));
        if (_DEBUG)
        std::cerr << "Razón: " << (error ? error : "Desconocido") << std::endl;

    }

    context = alcCreateContext(device, nullptr); // Create a context
    if (!context) {
        if (_DEBUG)
        std::cerr << "Error: No se pudo crear el contexto de audio. Razón: ";
        const ALCchar* error = alcGetString(device, alcGetError(device));
        if (_DEBUG)
        std::cerr << (error ? error : "Desconocido") << std::endl;
        alcCloseDevice(device);

    }

    if (!alcMakeContextCurrent(context)) { // Make the current context
        if (_DEBUG)
        std::cerr << "Error: No se pudo hacer el contexto actual. Razón: ";
        const ALCchar* error = alcGetString(device, alcGetError(device));
        if (_DEBUG)
        std::cerr << (error ? error : "Desconocido") << std::endl;
        alcDestroyContext(context);
        alcCloseDevice(device);

    }
    if (_DEBUG)
    std::cout << "OpenAL inicializado correctamente con el dispositivo: " << defaultDevice << std::endl;
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

