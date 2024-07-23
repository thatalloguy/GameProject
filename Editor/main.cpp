//
// Created by allos on 7/1/2024.
//

#define LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1


#include "spdlog/spdlog.h"
#include "src/App.h"
#include "Animations/AnimationManager.h"

int main() {

    Lake::App::Init();


    Lake::App::Run();



    Lake::App::CleanUp();



    return 0;
}

