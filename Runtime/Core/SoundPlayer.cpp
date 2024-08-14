//
// Created by allos on 8/14/2024.
//

#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(Quack::AudioEngine &audioEngine, Player &player) : _engine(audioEngine), _player(player) {
    loadSounds();
}

void SoundPlayer::loadSounds() {
    Quack::SoundCreationInfo _owl {
            .filePath = "../../Assets/Audio/effects/Ambient_1.mp3",
            .shouldLoop = false,
            .spatialBlend = 1.0f
    };

    _sounds.push_back(_engine.registerSound(_owl));
}

void SoundPlayer::update() {
    int i = 0;
    for (auto sound : _queue) {
        if (!_engine.isSoundPlaying(sound.id)) {
            _queue.erase(_queue.begin() + i);
            spdlog::info("removed");
        }
        _engine.setSoundPosition(sound.id, sound.position);
        auto dir = getDirection(sound.position);
        _engine.updateSound(sound.id, _player.position, dir);
        i++;
    }
}

void SoundPlayer::playRandomSound() {
    _queue.push_back({_sounds.back(), {-4, 0, 33}});
    _engine.playSound(_sounds.back());
}

Quack::Math::Vector3 SoundPlayer::getDirection(Quack::Math::Vector3 soundPos) {
    auto dir = glm::normalize(glm::vec3(_player._camera.getRotationMatrix() * glm::vec4(soundPos.x - _player.position.x  , _player.position.y - soundPos.y, _player.position.z - soundPos.z, 0)));

    return Quack::Math::Vector3{dir.x, dir.y, dir.z};
}
