//
// Created by allos on 7/1/2024.
//

#include "ProjectManager.h"

#include "LakeUtils.h"

#include <filesystem>
#include <fstream>


void Lake::ProjectManager::init(Lake::ProjectManagerCreateInfo *createInfo) {
    _version = createInfo->EditorVersion;

    _minCompatibleVer = createInfo->minCompatibleVer;
    _defaultProjPath = createInfo->defaultProjectPath;
}

bool Lake::ProjectManager::doesProjectExist(const char *projectName) {
    return std::filesystem::is_directory(std::string(_defaultProjPath) + projectName);
}

Lake::ProjectManagerError Lake::ProjectManager::createProject(const char *projectName, bool auto_open) {
    auto path = std::string(_defaultProjPath) + std::string(projectName);
    bool success = std::filesystem::create_directory(path);

    if (!success) {
        return Lake::ProjectManagerError::Failed_To_Create_Base_Folder;
    }

    // create other stuff
    std::filesystem::create_directory(path + std::string("/Levels/"));
    success = std::filesystem::create_directory(path + std::string("/Assets/"));
    if (!success) {
        return Lake::ProjectManagerError::Failed_To_Create_Asset_Folder;
    }


    path += "/Levels/test.lake";

    std::ofstream t(path);

    t.close();

    Lake::Utils::saveStructToFile(path.c_str(), config);

    Lake::Utils::getStructFromFile(path.c_str(), config);

    // auto open
    if (auto_open) {
        openProject(projectName);
    }
}

Lake::ProjectManagerError Lake::ProjectManager::openProject(const char *projectName) {
    if (!doesProjectExist(projectName)) {
        return Lake::ProjectManagerError::Failed_To_Open_Project;
    }

}

void Lake::ProjectManager::destroy() {

}

const char *Lake::ProjectManager::getErrorMessage(Lake::ProjectManagerError errorCode) {
    switch (errorCode) {

        case Lake::ProjectManagerError::Success:
            return "Success\n";

        case Lake::ProjectManagerError::Failed_To_Create_Base_Folder:
            return "Failed to create the Base project Folder\n";

        case Lake::ProjectManagerError::Failed_To_Create_Asset_Folder:
            return "Failed to create the Asset Folder\n";

        case Lake::ProjectManagerError::Unknown:
            return "Unknown error code ;-; \n";

        default:
            return "Unknown Error Code\n";

    }
}
