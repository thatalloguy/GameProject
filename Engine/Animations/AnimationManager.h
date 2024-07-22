//
// Created by allos on 7/22/2024.
//

#ifndef DUCKWATCHERS_ANIMATIONMANAGER_H
#define DUCKWATCHERS_ANIMATIONMANAGER_H

#include <vector>
#include <string>
#include <unordered_map>
#include "Math/Vecs.h"

namespace Quack {


    struct Keyframe {
        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 scale{0, 0, 0};
        Quack::Math::Vector3 rotation{0, 0, 0};
        float timePosition = 0;
    };

    struct Animation {
        std::vector<Keyframe> frames;
        int currentFrame = 0;
    };


    namespace AnimationUtils {

        void initAnimation(Animation& animation);
        void updateAnimation(Animation& animation, float currentTime);

    };

}

#endif //DUCKWATCHERS_ANIMATIONMANAGER_H
