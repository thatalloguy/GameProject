//
// Created by allos on 8/14/2024.
//

#ifndef DUCKWATCHERS_SOUNDPLAYER_H
#define DUCKWATCHERS_SOUNDPLAYER_H


#include "Audio/AudioEngine.h"
#include "FishingManager.h"
#include "Utils/Array.h"

class SoundPlayer {

public:

    SoundPlayer(Quack::AudioEngine& audioEngine, Player& player);
    ~SoundPlayer() = default;

    void loadSounds();

    void update();

private:

    Player& _player;
    Quack::AudioEngine& _engine;


    STL::Array<Quack::SoundID, 4> _queue;

};


#endif //DUCKWATCHERS_SOUNDPLAYER_H
