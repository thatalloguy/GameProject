//
// Created by allos on 7/22/2024.
//

#include "AnimationManager.h"
#include "spdlog/spdlog.h"


void Quack::AnimationUtils::updateAnimation(Quack::Animation &animation, float currentTime, Quack::Entity& entity) {

    // the animation is done. or the animation is too small ( less than 2 frames)
    if (animation.currentFrameIndex >= animation.frames.size() - 1) {
        //loop: animation.currentFrameIndex = 0;
        return;
    }

    auto currentFrame = animation.frames[animation.currentFrameIndex];
    auto nextFrame = animation.frames[animation.currentFrameIndex + 1];

    float begin = currentFrame.timePosition;
    float end = nextFrame.timePosition;

    float localTimeScale = 1 / (end - begin);

    float currentTimePos = currentTime - begin;


    //check if the animation is complete
    if (currentTime >= end) {
        animation.currentFrameIndex++;
        return;
    }


    currentTimePos = currentTimePos * localTimeScale;

    // linearly interpolate the position.
    /*
    animation.currentFrame.position.x = lerp(currentFrame.position.x, nextFrame.position.x, currentTimePos);
    animation.currentFrame.position.y = lerp(currentFrame.position.y, nextFrame.position.y, currentTimePos);
    animation.currentFrame.position.z = lerp(currentFrame.position.z, nextFrame.position.z, currentTimePos);
    */
    entity.position.x = lerp(currentFrame.position.x, nextFrame.position.x, currentTimePos);
    entity.position.y = lerp(currentFrame.position.y, nextFrame.position.y, currentTimePos);
    entity.position.z = lerp(currentFrame.position.z, nextFrame.position.z, currentTimePos);

}

float Quack::AnimationUtils::lerp(float a, float b, float t) {
    return a + t * (b - a);
}

Quack::AnimationID Quack::AnimationDataBase::registerAnimation(Quack::Animation *animation)  {

    AnimationID id = idCounter++;
    _animations.emplace(id, animation);

    return id;
}

void Quack::AnimationDataBase::cleanUp()  {
    for (auto pair : _animations) {
        delete pair.second;
    }
}

Quack::Animation *Quack::AnimationDataBase::getAnimation(Quack::AnimationID id) {
    return _animations[id];
}

void Quack::AnimationPlayer::animationTick(float dt)  {
    globalTime += tickSpeed * dt;
    for (auto pair : _animationQueue) {
        pair.first->time += tickSpeed * dt;
        AnimationUtils::updateAnimation(*pair.first, pair.first->time, *pair.second);
    }
}

void Quack::AnimationPlayer::animationTick(int currentFrame, int end) {
    for (auto pair : _animationQueue) {
        AnimationUtils::updateAnimation(*pair.first, (float) currentFrame / (float) end, *pair.second);
    }
}

void Quack::AnimationPlayer::playAnimation(Quack::Animation *animation, Quack::Entity *entity)  {
    _animationQueue.emplace_back(animation, entity);
}


void Quack::AnimationPlayer::resetTime(Quack::Animation &animation) {
    animation.time = 0.0f;
}