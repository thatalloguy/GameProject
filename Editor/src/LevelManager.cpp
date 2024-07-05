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
                // you .. are .. ugly
                loadLevel(file.path().generic_string().c_str(), file.path().filename().generic_string().substr(0, file.path().filename().generic_string().size() - 5).c_str());
            }
        }


    }
}

void Lake::LevelManager::newLevel(const char* name) {
    _levels.push_back({.name = name});
}
void Lake::LevelManager::selectLevel(const char *name) {
    for (auto level : _levels) {
        if (strcmp(name, level.name) == true) {
            _currentLevel = &level;
        }
    }
}

void Lake::LevelManager::loadLevel(const char* fileName, const char* name) {
    spdlog::debug("loading level {}", fileName);

    FILE* file = fopen(fileName, "r");
    if (file) {
        Quack::EntityBlueprint tempObject{};
        // will this work?
        //Lake::Utils::getStructArrayFromFile(assetDataFile, static_array, 1);

        _levels.push_back({.name = name});

        while (fread(&tempObject, sizeof(Quack::AssetInfo), 1, file) == 1) {
            _levels.back().entities.push_back(tempObject);
        }

        fclose(file);

    } else {
        spdlog::error("Cannot load handle: {}", name);
    }

    // load level
}
void Lake::LevelManager::exportData() {
    std::string path = _projectManager->getProjectPath();
    path += "/Levels/";

    for (auto level : _levels) {

        auto str = path + level.name;


        FILE* file = fopen(str.c_str(), "w");

        for (auto entity : level.entities) {
            fwrite(&entity, sizeof(Quack::EntityBlueprint), 1, file);
        }

        fclose(file);
    }
}



unsigned int Lake::LevelManager::newEntity() {
    return 0;
}
void Lake::LevelManager::deleteEntity(unsigned int ID) {

}
void Lake::LevelManager::selectEntity(unsigned int ID) {

}



void Lake::LevelManager::renderEntityTree() {

}

void Lake::LevelManager::renderEntityInfo() {

}
