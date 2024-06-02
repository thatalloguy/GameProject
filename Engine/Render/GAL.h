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

    typedef unsigned int GPUBufferId;


    enum class BufferType : unsigned int {
        Array
    };

    enum class BufferUsage : unsigned int {
        Static,
        Dynamic
    };



    struct RenderCreationInfo {

    };

    void init(RenderCreationInfo& creationInfo);
    void cleanUp();


    void clearScreen(Math::Vector4 color);
    void setViewport(Math::Vector4 rect);

    GPUBufferId& createVertexBuffer(void* data, int size, BufferUsage usage, BufferType type);

};


#endif //GAME_GAL_H
