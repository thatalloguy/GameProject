//
// Created by allos on 7/22/2024.
//

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include "Math/Vecs.h"
#include "Objects/Entity.h"



namespace Quack {


    typedef unsigned int AnimationID;

    struct Keyframe {
        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 scale{0, 0, 0};
        Quack::Math::Vector3 rotation{0, 0, 0};
        float timePosition = 0;
    };

    struct Animation {
        std::vector<Keyframe> frames;
        int currentFrameIndex = 0;
        float time = 0.0f;

        // the interpolated frame
        Keyframe currentFrame{};
        bool loop = false;
    };

    // using namespaces instead of classes here to make everything global and static no need for instances.

    namespace AnimationUtils {

        bool updateAnimation(Animation& animation, float currentTime, Quack::Entity& entity);

        void saveAnimation(std::ofstream& file, Animation& animation);
        void saveKeyframe(std::ofstream& file, Keyframe& keyframe);

        float lerp(float a, float b, float t);

    };

    class AnimationDataBase {

    public:

        AnimationID registerAnimation(Animation* animation);
        void cleanUp();

        Animation* getAnimation(AnimationID id);


        AnimationID idCounter = 0;
        std::unordered_map<AnimationID, Animation*> _animations;
    };

    class AnimationPlayer {

    public:
        // basically the update method;
        void animationTick(float dt);

        // currentFrame / end should ALWAYS == 0.0f-1.0f
        void animationTick(int currentFrame, int end);

        void playAnimation(Animation* animation, Quack::Entity* entity);


        void resetTime(Animation& animation);


    private:
        std::vector<std::pair<Animation*, Quack::Entity*>> _animationQueue;
        float globalTime = 0.0f;
        float tickSpeed = 1.0f;
    };

}

