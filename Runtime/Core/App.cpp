//
// Created by allos on 6/1/2024.
//

#include "App.h"
#include "Input/InputManager.h"
#include "spdlog/spdlog.h"


#define USE_GL
#include <Render/GAL.h>

namespace Game {

    namespace {
        GAL::Mesh* mesh = nullptr;
        GAL::GPUProgram shader;
    }

}

void App::init() {
                                   // Nothing wrong with this code, fight me :)
    window = new Quack::Window(*new Quack::WindowCreationData{});

    Quack::Input::setTargetWindow(*window);

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

    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
                                     "}\0";
    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
                                       "}\n\0";

    Game::shader = GAL::loadShader(vertexShaderSource, fragmentShaderSource);
    Game::mesh = GAL::createMesh(vertices, indices);

}

void App::run() {
    int count;
    while (!window->shouldShutdown()) {

        GAL::setViewport({0, 0, 1280, 720});
        GAL::clearScreen({0, 0.6f, 0, 1});


        GAL::drawMesh(*Game::mesh, Game::shader);

        if (Quack::Input::isKeyPressed(Quack::Key::A)) {
            spdlog::info("A KEY PRESSED");
        }

        //spdlog::info("Controller Axis: {} | {}", Quack::Input::getJoystickAxis(0).x, Quack::Input::getJoystickAxis(0).y);

        window->update();
    }

}

void App::cleanup() {
    delete window;
    delete Game::mesh;
    GAL::deleteGPUProgram(Game::shader);
}
