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

    class AudioEngine {


    public:

        void initialize();
        void destroy();


        void processEffect(unsigned int soundId, AudioBuffer& in);


    private:
        IPLContext _context;

    };



    struct SoundEffectInfo {
        AudioEngine* audioEngine;
        unsigned int soundId;
    };

}// namespace quack


#endif //GAME_AUDIOENGINE_H
