//
// Created by allos on 7/5/2024.
//

#include "LevelManager.h"
#include "spdlog/spdlog.h"


#include <string>
#include <filesystem>


void Lake::LevelManager::Initialize(Lake::ProjectManager *projectManager, bool shouldLoadLevels) {

    _projectManager = projectManager;

    // check if the file exists. then load?
    if (shouldLoadLevels) {
        // Get all of the files in the directory that end on .lake

        std::string path = _projectManager->getProjectPath();
        if (_projectManager->getProjectPath() == nullptr) return;


        path += "/Levels/"; // path = "Projects/PROJECT/Levels/"

        for (const auto& file : std::filesystem::directory_iterator(path)) {
            if (file.path().extension() == ".lake") {
                // you .. are .. ugly V
                loadLevel(file.path().generic_string().c_str());
            }
        }


    }
}

void Lake::LevelManager::newLevel() {
    _levels.push_back({.name = "newLevel"});
}


void Lake::LevelManager::loadLevel(const char* fileName) {
    spdlog::debug("loading level {}", fileName);

    // load level
}

void Lake::LevelManager::newEntity() {

}
void Lake::LevelManager::deleteEntity(unsigned int ID) {

}
void Lake::LevelManager::selectEntity(unsigned int ID) {

}

void Lake::LevelManager::renderUI(float windowWidth, float windowHeight) {

}

void Lake::LevelManager::exportData() {

}
