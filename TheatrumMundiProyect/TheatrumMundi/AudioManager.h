#pragma once

#include "al.h"
#include "alc.h"
#include <vector>
#include <string>

class AudioManager {
public:
    AudioManager();  
    ~AudioManager(); 


private:
    ALCdevice* device;          
    ALCcontext* context; 
    std::vector<ALuint> sources; 
    std::vector<ALuint> buffers; 
};

