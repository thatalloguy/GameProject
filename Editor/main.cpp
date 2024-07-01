//
// Created by allos on 7/1/2024.
//

#include <cstdio>
#define LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1
#include "src/ProjectManager.h"

int main() {


    //little test
    Lake::ProjectManagerCreateInfo projectManagerCreateInfo{};

    Lake::ProjectManager projectManager;

    projectManager.init(&projectManagerCreateInfo);


    if (!projectManager.doesProjectExist("TestLevel")) {
        projectManager.createProject("TestLevel");

        // do the rest

    }else {
    }


    return 0;
}

