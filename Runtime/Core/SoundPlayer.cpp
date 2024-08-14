//
// Created by allos on 8/14/2024.
//

#include "SoundPlayer.h"

SoundPlayer::SoundPlayer(Quack::AudioEngine &audioEngine, Player &player) : _engine(audioEngine), _player(player) {
    loadSounds();
}

void SoundPlayer::loadSounds() {
    Quack::SoundCreationInfo _owl {
            .filePath = "../../Assets/Audio/effects/owl_1.mp3",
            .shouldLoop = false,
            .spatialBlend = 1.0f
    };

    _sounds.emplace_back(_engine.registerSound(_owl), Quack::Math::Vector3{-3, 0, 0});
}

void SoundPlayer::update() {
    int i = 0;
            Quack::Math::Vector3 dir = getDirection(_queue[i].second);
            _engine.updateSound(_queue[i].first, _player.position, dir);
            _engine.setSoundPosition(_queue[i].first, _queue[i].second);

}

void SoundPlayer::playRandomSound() {

    _queue.emplace_back(_sounds[0]);
    _engine.playSound(_sounds[0].first);
}

Quack::Math::Vector3 SoundPlayer::getDirection(Quack::Math::Vector3 soundPos) {
    auto dir = glm::normalize(glm::vec3(_player._camera.getRotationMatrix() * glm::vec4(soundPos.x - _player.position.x  , _player.position.y - soundPos.y, _player.position.z - soundPos.z, 0)));

    return Quack::Math::Vector3{dir.x, dir.y, dir.z};
}
