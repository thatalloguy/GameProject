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
    //glClearColor(color.x, color.y, color.z, color.w);
    glClear(GL_COLOR_BUFFER_BIT );
}

void GAL::setViewport(Math::Vector4 rect) {
    glViewport(rect.x, rect.y, rect.z, rect.w);
}

GAL::Mesh *GAL::createMesh(float vertices[],unsigned int indices[]) {


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

void GAL::drawMesh(GAL::Mesh &mesh, GPUProgram program) {

    glUseProgram(program);
    glBindVertexArray(mesh.VAO);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

GAL::GPUProgram GAL::loadShader(const char *vert_src, const char *frag_src, bool enable_errors) {

    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int program;

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vert_src, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &frag_src, NULL);
    glCompileShader(fragmentShader);

    //error checking
    if (enable_errors) {
        int success;
        char info[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, info);
            spdlog::error("[Shader Compilation] {}", info);
        }

        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

        if(!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, info);
            spdlog::error("[Shader Compilation] {}", info);
        }
    }

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    //Check for linking errors

    if (enable_errors) {
        int success;
        char info[512];
        glGetProgramiv(program, GL_LINK_STATUS, &success);

        if(!success) {
            glGetProgramInfoLog(program, 512, NULL, info);
            spdlog::error("[Shader Linking] {}", info);
        }
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

void GAL::deleteGPUProgram(GAL::GPUProgram program) {
    glDeleteProgram(program);
}

GAL::Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
