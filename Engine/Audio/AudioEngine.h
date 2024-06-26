//
// Created by allos on 6/26/2024.
//

#ifndef GAME_AUDIOENGINE_H
#define GAME_AUDIOENGINE_H


#include <phonon.h>

namespace Quack {


    struct AudioBuffer{
        int channel;
        float* data;
        int length;
    };

    struct SoundCreateInfo {
        int length;
        int channel = 2;
        int frameSize;
    };

    struct SoundCache {
        IPLAudioBuffer inBuffer;
        IPLAudioBuffer outBuffer;
        IPLHRTF hrtf;
        IPLBinauralEffect effect;
    };

    class AudioEngine {


    public:

        void initialize();
        void destroy();

        unsigned int registerSound(SoundCreateInfo& info);

        void processEffect(unsigned int soundId, AudioBuffer& in);


    private:
        IPLContext _context;
        SoundCache bufferInfo; // temp
        unsigned int idCount = 0;

        void destroySoundBufferCache(SoundCache& soundBufferInfo);
    };



    struct EffectUserData {
        AudioEngine* audioEngine;
        unsigned int soundId;
    };

}// namespace quack


#endif //GAME_AUDIOENGINE_H
