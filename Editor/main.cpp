//
// Created by allos on 7/1/2024.
//

#define LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1


#include "spdlog/spdlog.h"
#include "src/App.h"
#include "Animations/AnimationManager.h"

int main() {
/*
    Lake::App::Init();


    Lake::App::Run();



    Lake::App::CleanUp();*/

    Quack::Animation testAnimation{
            {
                Quack::Keyframe{.position={1, 1, 1}, .timePosition=0.0f},
                Quack::Keyframe{.position={4, -5, 7}, .timePosition=4.0f}
                   }
    };


    Quack::AnimationUtils::initAnimation(testAnimation);

    for (float i=0.0f; i<5.0f; i+=0.1f) {
        Quack::AnimationUtils::updateAnimation(testAnimation, i);
        spdlog::info("Current pos: {} {} {}", testAnimation.currentFrame.position.x, testAnimation.currentFrame.position.y, testAnimation.currentFrame.position.z);
    }



    return 0;
}

