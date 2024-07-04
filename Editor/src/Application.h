//
// Created by allos on 7/2/2024.
//

#ifndef GAME_APPLICATION_H
#define GAME_APPLICATION_H


#include <Video/Window.h>
#include "ProjectManager.h"
#include "imgui.h"
#include "AssetManager.h"


namespace Lake {


    class Application {
    public:

        void Init(ProjectManager* projectManager, AssetManager* assetManager);

        void Run();

        void Destroy();

    private:
        void loadImGuiStyle();
        void loadImGuiFont();

        static void dropCallback(GLFWwindow* window, int count, const char** paths);


        Quack::Window* _window;

        Lake::ProjectManager* _projectMang;
        Lake::AssetManager* _assetManager;
        VulkanEngine* _renderer;
        ImFont* icon;
    };

}// Lake
#endif //GAME_APPLICATION_H
