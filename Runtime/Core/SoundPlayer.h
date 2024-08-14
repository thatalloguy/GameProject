//
// Created by allos on 8/14/2024.
//

#ifndef DUCKWATCHERS_SOUNDPLAYER_H
#define DUCKWATCHERS_SOUNDPLAYER_H

#ifndef PLAY_LOCATION_AMOUNT
#define PLAY_LOCATION_AMOUNT 4
#endif

#include "FishingManager.h"
#include "Audio/AudioEngine.h"
#include "Utils/Array.h"

struct SoundEffect {
    Quack::SoundID id;
    Quack::Math::Vector3 position;
};

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

    std::vector<Quack::SoundID> _sounds;
    std::vector<SoundEffect> _queue;

    Quack::Math::Vector3 playPositions[PLAY_LOCATION_AMOUNT] = {{-27, 2, -9}, {27, 2, -11}, {-15, 2, -19}, {0, 2, 84}};
};


#endif //DUCKWATCHERS_SOUNDPLAYER_H
