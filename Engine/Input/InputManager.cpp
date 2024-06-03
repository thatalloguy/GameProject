//
// Created by allos on 6/3/2024.
//

#include "InputManager.h"


namespace Quack {


    static Window* currentWindow = nullptr;


    void Quack::Input::setTargetWindow(Quack::Window &window) {
        currentWindow = &window;
    }

    bool Quack::Input::isKeyPressed(Quack::Key key) {
        return (glfwGetKey(currentWindow->getRawWindow(), static_cast<int>(key)) == GLFW_PRESS);
    }

    int Input::isControllerPresent(int controllerId) {
        return glfwJoystickPresent(controllerId);
    }

    Math::Vector2 Input::getJoystickAxis(int controllerId) {
        int count;
        return Math::Vector2(glfwGetJoystickAxes(controllerId, &count)[0], glfwGetJoystickAxes(controllerId, &count)[1]);
    }
}

