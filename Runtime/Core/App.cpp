//
// Created by allos on 6/1/2024.
//

#include "App.h"


#define USE_GL
#include <Render/GAL.h>

namespace Game {

    namespace {
        GAL::Mesh* mesh = nullptr;
    }

}

void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});

    GAL::RenderCreationInfo info{};

    GAL::init(info);

    float vertices[] = {
            0.5f,  0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    Game::mesh = GAL::createMesh(vertices, indices);

}

void App::run() {

    while (!window->shouldShutdown()) {

        GAL::setViewport({0, 0, 1280, 720});
        GAL::clearScreen({0, 0.6f, 0, 1});

        GAL::drawMesh(*Game::mesh);

        window->update();
    }

}

void App::cleanup() {
    delete window;
    delete Game::mesh;
}
