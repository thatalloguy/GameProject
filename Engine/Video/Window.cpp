//
// Created by allos on 6/1/2024.
//

#include <glad/glad.h>
#include "Window.h"


#include "spdlog/spdlog.h"

#ifdef _WIN32

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3native.h"

#include <windows.h>
#include <dwmapi.h>

#endif


namespace Quack {


    Window::Window(WindowCreationData &creation_info) {
        window_info = &creation_info;

        // init glfw

        if (!glfwInit()) {
            spdlog::error("[WINDOW] Could not load GLFW");
            exit(-100);
        }

        spdlog::debug("[WINDOW] loaded Glfw");

        raw_window = glfwCreateWindow(creation_info.size.x, creation_info.size.y, creation_info.title, NULL, NULL);

        if (!raw_window) {
            spdlog::error("Could not create window");
        }

        glfwMakeContextCurrent(raw_window);
        gladLoadGL();

        // Render settings
        glViewport(0, 0, creation_info.size.x, creation_info.size.y);

        if (creation_info.should_maximize) {
            glfwMaximizeWindow(raw_window);
        }

        pushWindowMode(creation_info.starting_mode);


        // Do darktheme WINDOWS ONLY
#ifdef _WIN32

        BOOL USE_DARK_MODE = true;

        BOOL status = SUCCEEDED(DwmSetWindowAttribute(
                glfwGetWin32Window(raw_window), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                &USE_DARK_MODE, sizeof(USE_DARK_MODE)
        ));

#endif
    }

    Window::~Window() {
        glfwDestroyWindow(raw_window);

        glfwTerminate();

        delete window_info;
    }

    bool Window::shouldShutdown() {
        return (glfwWindowShouldClose(raw_window) || should_shutdown);
    }

    void Window::requestShutdown() {
        should_shutdown = true;
    }

    void Window::load_glfw_config() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
    }

    void Window::pushWindowMode(WindowMode mode) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);

        switch (mode) {
            case WindowMode::Fullscreen:
                glfwSetWindowMonitor(raw_window, monitor, 0, 0, vid_mode->width, vid_mode->height, vid_mode->refreshRate);
                break;

            case WindowMode::Windowed:
                glfwSetWindowMonitor(raw_window, NULL, 100, 100, window_info->size.x, window_info->size.y, vid_mode->refreshRate);
                break;
        }
    }

    void Window::update() {

        //temp
        glClearColor(0, 0.3, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        glfwPollEvents();
        glfwSwapBuffers(raw_window);
    }


} // Quack