//
// Created by allos on 7/3/2024.
//

#pragma once

#include <Math/Vecs.h>

namespace Quack {

    enum class ShapeType : unsigned int {
        Box = 0,
        Sphere = 1,
        Capsule = 2
    };

    struct EntityBlueprint {
        Math::Vector3 position{0, 0, 0};
        Math::Vector3 size{1, 1, 1};

        unsigned int model = 0;
        bool isDynamic = false;
        bool isPhysical = false;
        // Physicscreation
        Math::Vector4 rotation; // quaternion, but im too lazy to actually make a quaternion class
        ShapeType shapeType = ShapeType::Box;
        bool shouldActivate = false;
        unsigned int objectLayer;
        // motion = isDynamic :)
    };


    enum class AssetType : unsigned int {
        Model = 0,
        Sound = 1,
        Unknown = 2
    };

    struct AssetInfo {
        AssetType type = AssetType::Unknown;
        char fileName[54] = "Unknown";
        unsigned int Id = 0;
    };

    class Level {

    };

}