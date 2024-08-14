//
// Created by allos on 8/14/2024.
//

#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(Quack::AudioEngine &audioEngine, Player &player) : _engine(audioEngine), _player(player) {
    loadSounds();
}

void SoundPlayer::loadSounds() {

    _sounds.push_back(_engine.registerSound({
                                                    .filePath = "../../Assets/Audio/effects/Ambient_1.mp3",
                                                    .shouldLoop = false
                                            }));
}

void SoundPlayer::update() {
    int i = 0;
    for (auto sound : _queue) {
        if (!_engine.isSoundPlaying(sound.id)) {
            _queue.erase(_queue.begin() + i);
            spdlog::info("removed Sound effect from queue");
        }
        _engine.setSoundPosition(sound.id, sound.position);
        auto dir = getDirection(sound.position);
        _engine.updateSound(sound.id, _player.position, dir);
        i++;
    }
}

void SoundPlayer::playRandomSound() {

    int randomSound = std::rand() % _sounds.size();
    int randomLoc = std::rand() % PLAY_LOCATION_AMOUNT;

    auto sound = _sounds[randomSound];
    auto position = playPositions[randomLoc];

    spdlog::info("Playing {} at {} {} {} || {} {}", sound, position.x, position.y, position.z, randomSound, randomLoc);

    _queue.push_back({sound, position});
    _engine.playSound(sound);
}

Quack::Math::Vector3 SoundPlayer::getDirection(Quack::Math::Vector3 soundPos) {
    auto dir = glm::normalize(glm::vec3(_player._camera.getRotationMatrix() * glm::vec4(soundPos.x - _player.position.x  , _player.position.y - soundPos.y, _player.position.z - soundPos.z, 0)));

    return Quack::Math::Vector3{dir.x, dir.y, dir.z};
}
