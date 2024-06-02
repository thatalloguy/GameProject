//
// Created by allos on 6/2/2024.
//

#include "GAL.h"
#include "GLFW/glfw3.h"
#include <glad/glad.h>
#include <spdlog/spdlog.h>

void GAL::init(GAL::RenderCreationInfo &creationInfo) {

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
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