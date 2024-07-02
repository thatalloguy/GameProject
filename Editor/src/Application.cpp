//
// Created by allos on 7/2/2024.
//

#include "Application.h"

void Lake::Application::Init() {

    Quack::WindowCreationData windowCreationData {
            .title = "Lake Editor Version 0.0.1"
    };

    _window = new Quack::Window(windowCreationData);
}

void Lake::Application::Run() {

    while (!_window->shouldShutdown()) {

        _window->update();
    }

}

void Lake::Application::Destroy() {
    delete _window;
}
