//
// Created by allos on 6/1/2024.
//
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

        spdlog::info("[WINDOW] loaded Glfw");
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        raw_window = glfwCreateWindow((int) creation_info.size.x, (int)creation_info.size.y, creation_info.title, nullptr, nullptr);

        if (!raw_window) {
            spdlog::error("Could not create window");
        }

        if (creation_info.should_maximize) {
            glfwMaximizeWindow(raw_window);
        }

        pushWindowMode(creation_info.starting_mode);


        // Do darktheme WINDOWS ONLY
#ifdef _WIN32

        BOOL USE_DARK_MODE = true;

        DwmSetWindowAttribute(
                glfwGetWin32Window(raw_window), DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE,
                &USE_DARK_MODE, sizeof(USE_DARK_MODE)
        );

#endif
    }

    Window::~Window() {
        glfwDestroyWindow(raw_window);

        glfwTerminate();

    }

    bool Window::shouldShutdown() {
        return (glfwWindowShouldClose(raw_window) || should_shutdown);
    }

    void Window::requestShutdown() {
        should_shutdown = true;
    }


    void Window::pushWindowMode(WindowMode mode) {
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vid_mode = glfwGetVideoMode(monitor);

        switch (mode) {
            case WindowMode::Fullscreen:
                glfwSetWindowMonitor(raw_window, monitor, 0, 0, vid_mode->width, vid_mode->height, vid_mode->refreshRate);
                break;

            case WindowMode::Windowed:
                glfwSetWindowMonitor(raw_window, nullptr, 100, 100, (int) window_info->size.x, (int) window_info->size.y, vid_mode->refreshRate);
                break;
        }
    }

    void Window::update() {
        glfwSwapBuffers(raw_window);
        glfwPollEvents();
    }

    GLFWwindow *Window::getRawWindow() {
        return raw_window;
    }

    Quack::Math::Vector2 Window::getSize() {
        int w, h;
        glfwGetWindowSize(raw_window, &w, &h);
        return Math::Vector2(w, h);
    }


} // Quack