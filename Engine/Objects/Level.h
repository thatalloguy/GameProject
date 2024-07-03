//
// Created by allos on 7/3/2024.
//

#pragma once

namespace Quack {

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