//
// Created by allos on 7/1/2024.
//

#include <cstdio>
#define LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1
#include "src/ProjectManager.h"
#include "spdlog/spdlog.h"

int main() {


    //little test
    Lake::ProjectManagerCreateInfo projectManagerCreateInfo{};

    Lake::ProjectManager projectManager;

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
        spdlog::info("Project name: {}", projectManager._config.ProjectName);
    }


    return 0;
}

