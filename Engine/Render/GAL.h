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

//todo remove this

namespace GAL {

    struct Mesh {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        ~Mesh();
    };

    typedef unsigned int GPUProgram;

    struct RenderCreationInfo {

    };

    void init(RenderCreationInfo& creationInfo);
    void cleanUp();


    void clearScreen(Math::Vector4 color);
    void setViewport(Math::Vector4 rect);

    //USER MUST DELETE THESE THEMSELF
    Mesh* createMesh(float vertices[],unsigned int indices[]);
    void drawMesh(Mesh& mesh, GPUProgram program);

    GPUProgram loadShader(const char* vert_src, const char* frag_src, bool enable_errors=true);
    void deleteGPUProgram(GPUProgram program);


};


#endif //GAME_GAL_H
