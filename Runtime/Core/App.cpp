//
// Created by allos on 6/1/2024.
//

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"


void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});
    Quack::Input::setTargetWindow(*window);


}

void App::run() {
    int count;
    while (!window->shouldShutdown()) {

        if (Quack::Input::isKeyPressed(Quack::Key::A)) {
            spdlog::info("A KEY PRESSED");
        }

        if (Quack::Input::isButtonPressed(0, 2)) {
            spdlog::info("A BUTTON PRESSED");
        }

        window->update();
    }

}

void App::cleanup() {
    delete window;
}
