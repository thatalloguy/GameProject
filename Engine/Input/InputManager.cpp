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

    Math::Vector4 Input::getJoystickAxis(int controllerId) {
        int count;
        auto axis = glfwGetJoystickAxes(controllerId, &count);
        return Math::Vector4(axis[0], axis[1], axis[2], axis[3]);
    }

    bool Input::isButtonPressed(int controllerId, int button) {
        int count;
        return (glfwGetJoystickButtons(controllerId, &count)[button] == GLFW_PRESS);
    }

    void Input::setMouseMode(MouseMode mode) {
        glfwSetInputMode(currentWindow->getRawWindow(), GLFW_CURSOR, static_cast<int>(mode));
    }

    Quack::Math::Vector2 Input::getMousePos() {
        double x;
        double y;

        glfwGetCursorPos(currentWindow->getRawWindow(), &x, &y);

        return {(float) x, (float) y};
    }
}

