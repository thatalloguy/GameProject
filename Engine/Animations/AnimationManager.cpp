//
// Created by allos on 7/22/2024.
//

#include "AnimationManager.h"

void Quack::AnimationUtils::initAnimation(Quack::Animation &animation) {


    // get the first and last time point


}

void Quack::AnimationUtils::updateAnimation(Quack::Animation &animation, float currentTime) {

    if (animation.currentFrame >= animation.frames.size()) {
        animation.currentFrame = 0;
        return
    }

    auto currentFrame = animation.frames[animation.currentFrame];
    auto nextFrame = animation.frames[animation.currentFrame + 1];

    float begin = currentFrame.timePosition;
    float end = nextFrame.timePosition;

    //check if the animation is complete
    if (currentTime >= end) {
        animation.currentFrame++;
        return;
    }

    float localTimeScale = 1 / (end - begin);

    float currentTimePos = currentTime - begin;




}
