//
// Created by allos on 7/1/2024.
//

#define LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1


#include <Users/allos/source/Game/Editor/src/AssetManager.h>
#include "src/ProjectManager.h"

#include "spdlog/spdlog.h"
#include "src/Application.h"

int main() {


    //little test
    Lake::ProjectManagerCreateInfo projectManagerCreateInfo{};

    Lake::ProjectManager projectManager;
    Lake::AssetManager assetManager;



    projectManager.init(&projectManagerCreateInfo);


    if (!projectManager.doesProjectExist("TestLevel")) {

        spdlog::info("Folder does not exist!");

        if (projectManager.createProject("TestLevel") == Lake::ProjectManagerError::Success) {
            spdlog::info("Yippie");
        };

        // do the rest

    } else {
        spdlog::info("Folder does exist!");

        projectManager.openProject("../../Projects/TestLevel");

    }

    assetManager.Initialize("../../Projects/TestLevel/Assets/assetData.lake");




    Lake::Application app;

    app.Init(&projectManager, &assetManager);
    app.Run();
    app.Destroy();


    return 0;
}

