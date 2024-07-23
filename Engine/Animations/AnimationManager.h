//
// Created by allos on 7/22/2024.
//

#ifndef DUCKWATCHERS_ANIMATIONMANAGER_H
#define DUCKWATCHERS_ANIMATIONMANAGER_H

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
    };

    // using namespaces instead of classes here to make everything global and static no need for instances.

    namespace AnimationUtils {

        void updateAnimation(Animation& animation, float currentTime, Quack::Entity& entity);

        float lerp(float a, float b, float t);

    };

    namespace AnimationDataBase {

        AnimationID idCounter = 0;


        std::unordered_map<AnimationID, Animation*> _animations;

        //use ref instead of ptr to avoid having to check for a nullptr.
        AnimationID registerAnimation(Animation& animation) {

            AnimationID id = idCounter++;
            _animations.emplace(id, &animation);

            return id;
        }


    }

    namespace AnimationPlayer {

        std::vector<std::pair<Animation*, Quack::Entity*>> _animationQueue;
        float globalTime = 0.0f;
        float tickSpeed = 1.0f;


        // basically the update method;
        void animationTick(float dt) {
            globalTime += tickSpeed * dt;
            for (auto pair : _animationQueue) {
                pair.first->time += tickSpeed * dt;
                AnimationUtils::updateAnimation(*pair.first, pair.first->time, *pair.second);
            }
        }

        // currentFrame / end should ALWAYS == 0.0f-1.0f
        void animationTick(int currentFrame, int end) {
            for (auto pair : _animationQueue) {
                AnimationUtils::updateAnimation(*pair.first, (float) currentFrame / (float) end, *pair.second);
            }
        }

        void playAnimation(Animation* animation, Quack::Entity* entity) {
            _animationQueue.emplace_back(animation, entity);
        }

        void playAnimation(AnimationID id, Quack::Entity* entity) {
            playAnimation(AnimationDataBase::_animations[id], entity);
        }

        void resetTime(Animation& animation) {
            animation.time = 0.0f;
        }

        void resetTime(AnimationID id) {
            resetTime(*AnimationDataBase::_animations[id]);
        }
    }

}

#endif //DUCKWATCHERS_ANIMATIONMANAGER_H
