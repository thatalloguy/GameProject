//
// Created by allos on 7/22/2024.
//

#include "AnimationManager.h"
#include "spdlog/spdlog.h"

void Quack::AnimationUtils::initAnimation(Quack::Animation &animation) {


    // get the first and last time point


}

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
