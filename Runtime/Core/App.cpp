//
// Created by allos on 6/1/2024.
//

#include "App.h"

void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});
}

void App::run() {

    while (!window->shouldShutdown()) {
        window->update();
    }

}

void App::cleanup() {
    delete window;
}
