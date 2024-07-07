//
// Created by allos on 7/2/2024.
//

#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H


#include <Video/Window.h>
#include "ProjectManager.h"
#include "imgui.h"
#include "AssetManager.h"
#include "EntityManager.h"


namespace Lake {


    class Application {
    public:

        void Init(ProjectManager* projectManager, AssetManager* assetManager, EntityManager* entityManager);

        void Run();

        void Destroy();

    private:
        void loadImGuiStyle();
        void loadImGuiFont();

        static void dropCallback(GLFWwindow* window, int count, const char** paths);

        void renderEditorTab();
        void renderConsoleTab();

        Quack::Window* _window;

        Lake::ProjectManager* _projectMang;
        Lake::AssetManager* _assetManager;
        Lake::EntityManager* _entityManager;
        spdlog::logger* _logger;
        VulkanEngine* _renderer;
        ImFont* icon;

        Quack::Math::Vector2 _editorWindowSize;
        float _editorWindowOffset = 0.75f; // goes from 0.75f to 1.0f;
        float _consoleWindowOffset = 0.75f; // goes from 0.75f to 1.0f;

        bool _renderEditor = true;
        bool _renderConsole = true;

        bool toggle = false;

        ImGuiTextBuffer consoleBuffer;
    };

}// Lake
#endif //GAME_APPLICATION_H
