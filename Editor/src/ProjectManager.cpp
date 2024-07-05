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
    success = std::filesystem::create_directory(path + std::string("/Assets/"));
    if (!success) {
        return Lake::ProjectManagerError::Failed_To_Create_Asset_Folder;
    }

    auto str = path + "/Assets/assetData.lake";

    FILE* file = fopen(str.c_str(), "w");

    fclose(file);

    str = path + "/objs.lake";

    file = fopen(str.c_str(), "w");

    fclose(file);


    path += "/config.lake";

    _config.projectVersion = (int) _version;

    Lake::Utils::saveStructToFile(path.c_str(), _config);


    // auto open
    if (auto_open) {
        path = std::string(_defaultProjPath) + projectName;
        return openProject(path.c_str());
    }

    return Lake::ProjectManagerError::Success;
}

Lake::ProjectManagerError Lake::ProjectManager::openProject(const char *projectName) {

    // do some checks if the editor can actually open the project.
    if (!doesPathExist(projectName)) {
        return Lake::ProjectManagerError::Failed_To_Open_Project;
    }

    auto path = std::string(projectName) + "/config.lake";
    Lake::Utils::getStructFromFile(path.c_str(), _readingProjectConfig);

    if (_readingProjectConfig.projectVersion < _minCompatibleVer || _readingProjectConfig.projectVersion < 0) {
        return Lake::ProjectManagerError::Project_Incompatible_With_Editor;
    }

    _currentProject = projectName;
    _isProjectOpen = true;

    _config = _readingProjectConfig;

    return Lake::ProjectManagerError::Success;
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
            return "Unknown error ;-; \n";

        case Lake::ProjectManagerError::Project_Incompatible_With_Editor:
            return "Project is incompatible with the Editor\n";

        default:
            return "Unknown Error Code\n";

    }
}

bool Lake::ProjectManager::doesPathExist(const char *projectName) {
    return std::filesystem::is_directory(projectName);
}

void Lake::ProjectManager::copyFileToProjectAssetFolder(const char *filePath, const char* fileName) {

    auto finalPath = std::string(_currentProject) + "/Assets/";

    std::filesystem::copy_file(filePath, finalPath + fileName);
}
