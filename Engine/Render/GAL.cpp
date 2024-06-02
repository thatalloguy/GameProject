//
// Created by allos on 6/2/2024.
//

#include "GAL.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>





void GAL::init(GAL::RenderCreationInfo &creationInfo) {

    if (!gladLoadGL()) {
        spdlog::debug("Could not load glad");
        exit(-102);
    }

}

void GAL::cleanUp() {
    //todo
}

void GAL::clearScreen(Math::Vector4 color) {
    glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GAL::setViewport(Math::Vector4 rect) {
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

GAL::Mesh *GAL::createMesh(void *vertices, void *indices) {


    Mesh* mesh = new Mesh();

    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    glBindVertexArray(mesh->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);


    return mesh;
}

void GAL::drawMesh(GAL::Mesh &mesh) {


    glBindVertexArray(mesh.VAO);
    // temp
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GAL::ShaderRef *GAL::loadShader(const char *vert_src, const char *frag_src) {

    ShaderRef* shader = new ShaderRef();


    return nullptr;
}

