//
// Created by allos on 6/26/2024.
//

#ifndef GAME_AUDIOENGINE_H
#define GAME_AUDIOENGINE_H


namespace Quack {



    class AudioEngine {


    public:

        void initialize();
        void destroy();

        void processEffect(AudioEngine* engine, unsigned int soundId);

    };



    struct SoundEffectInfo {
        AudioEngine* audioEngine;
        unsigned int soundId;
    };

}// namespace quack


#endif //GAME_AUDIOENGINE_H
