//
// Created by allos on 6/1/2024.
//

#ifndef GAME_WINDOW_H
#define GAME_WINDOW_H

#include <Utils/Defines.h>
#include <Math/Vecs.h>

#include <Renderer/VkEngine.h>

namespace Quack {

    enum class WindowMode {
        Windowed   = 0,
        Fullscreen = 1
    };

    // Info for the init phase of the window
    struct WindowCreationData {
        WindowMode starting_mode = WindowMode::Windowed;
        cstr title = "Quack quack";
        Math::Vector2 size = {1280, 720};
        bool should_maximize = false;
    };

    class Window {

    public:

        explicit Window(WindowCreationData& creation_info);
        ~Window();


        void requestShutdown();

        bool shouldShutdown();

        void pushWindowMode(WindowMode mode);

        void update();

        GLFWwindow* getRawWindow();

        Quack::Math::Vector2 getSize();

    private:

        WindowCreationData* window_info;
        GLFWwindow* raw_window;

        bool should_shutdown = false;
    };

} // Quack

#endif //GAME_WINDOW_H
