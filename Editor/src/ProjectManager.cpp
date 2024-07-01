//
// Created by allos on 7/1/2024.
//

#include "ProjectManager.h"

void Lake::ProjectManager::init(Lake::ProjectManagerCreateInfo *createInfo) {
    _version = createInfo->EditorVersion;

    _minCompatibleVer = createInfo->minCompatibleVer;
    _defaultProjPath = createInfo->defaultProjectPath;
}

bool Lake::ProjectManager::doesProjectExist(const char *projectName) {
    return false;
}

void Lake::ProjectManager::createProject(const char *projectName, bool auto_open) {

}

void Lake::ProjectManager::openProject(const char *projectName) {

}

void Lake::ProjectManager::destroy() {

}
