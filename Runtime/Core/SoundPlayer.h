//
// Created by allos on 8/14/2024.
//

#ifndef DUCKWATCHERS_SOUNDPLAYER_H
#define DUCKWATCHERS_SOUNDPLAYER_H


#include "FishingManager.h"
#include "Audio/AudioEngine.h"
#include "Utils/Array.h"

class SoundPlayer {

public:

    SoundPlayer(Quack::AudioEngine& audioEngine, Player& player);
    ~SoundPlayer() = default;

    void loadSounds();

    void playRandomSound();

    void update();

private:


    Quack::Math::Vector3 getDirection(Quack::Math::Vector3 soundPos);

    Player& _player;
    Quack::AudioEngine& _engine;

    std::vector<std::pair<Quack::SoundID, Quack::Math::Vector3>> _sounds;
    std::vector<std::pair<Quack::SoundID, Quack::Math::Vector3>> _queue;
};


#endif //DUCKWATCHERS_SOUNDPLAYER_H
