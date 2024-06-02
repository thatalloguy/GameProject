//
// Created by allos on 6/1/2024.
//

#include "App.h"


#define USE_GL
#include <Render/GAL.h>

void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});

    GAL::RenderCreationInfo info{};

    GAL::init(info);
}

void App::run() {

    while (!window->shouldShutdown()) {

        GAL::setViewport({0, 0, 1280, 720});
        GAL::clearScreen({0, 0.6f, 0, 1});

        window->update();
    }

}

void App::cleanup() {
    delete window;
}
