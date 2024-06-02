//
// Created by allos on 6/2/2024.
//


/*
 * GAL is a graphics abstraction layer made by allo 2024 (C)
 * TODO: more info
 */

#ifndef GAME_GAL_H
#define GAME_GAL_H

#include <Math/Vecs.h>

using namespace Quack;

namespace GAL {



    struct RenderCreationInfo {

    };

    void init(RenderCreationInfo& creationInfo);
    void cleanUp();



    void clearScreen(Math::Vector4 color);
    void setViewport(Math::Vector4 rect);

};


#endif //GAME_GAL_H
