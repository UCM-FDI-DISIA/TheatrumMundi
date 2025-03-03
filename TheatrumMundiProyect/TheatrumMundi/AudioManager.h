#pragma once

#include "al.h"
#include "alc.h"
#include "sndfile.h"
#include <vector>
#include <string>


// Class that represents a specific sound and allows total abstraction over the AudioManager
class Sound {
public:
    Sound(const std::string& filePath);
    ~Sound();
    ALuint getSource() const;
    ALuint getBuffer() const;
private:
    ALuint source;
    ALuint buffer;
};

class AudioManager {
public:

    // AudioManager is a singleton
    AudioManager(const AudioManager&) = delete;
    AudioManager& operator=(const AudioManager&) = delete;

    static AudioManager& Instance();

    
    ALuint loadWAV(const std::string& filePath);
    ALuint createSource();

    void setSourcePosition(Sound source, float x, float y, float z);
    void setListenerPosition(float x, float y, float z);

    
    void playSound(Sound source);
    void stopSound(Sound source);
    void pauseSound(Sound source);
    void resumeSound(Sound source);

    void setLooping(Sound source, bool loop);

private:

    AudioManager();
    ~AudioManager();

    ALCdevice* device;          
    ALCcontext* context; 
    std::vector<ALuint> sources; 
    std::vector<ALuint> buffers; 

};

