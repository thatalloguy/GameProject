//
// Created by allos on 7/2/2024.
//

#include "Application.h"

void Lake::Application::Init(ProjectManager* projectManager) {

    Quack::WindowCreationData windowCreationData {
            .title = "Lake Editor Version 0.0.1"
    };

    _window = new Quack::Window(windowCreationData);

    _renderer = new VulkanEngine();

    _renderer->Init(_window->getRawWindow());
}

void Lake::Application::Run() {

    while (!_window->shouldShutdown()) {

        _renderer->Run();
        _window->update();
    }

}

void Lake::Application::Destroy() {

    delete _window;

    _renderer->CleanUp();
    delete _renderer;

}
