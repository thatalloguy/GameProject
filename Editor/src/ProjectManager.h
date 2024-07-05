//
// Created by allos on 7/1/2024.
//

//Falllback defines.
#include <string>

#ifndef LAKE_EDITOR_VERSION_DATA
#define LAKE_EDITOR_VERSION 1
#define LAKE_EDITOR_MIN_COMPATIBLE_VERSION 1
#endif

#ifndef GAME_PROJECTMANAGER_H
#define GAME_PROJECTMANAGER_H


namespace Lake {

    struct ProjectManagerCreateInfo {
        unsigned int EditorVersion = LAKE_EDITOR_VERSION;
        unsigned int minCompatibleVer = LAKE_EDITOR_MIN_COMPATIBLE_VERSION;
        const char* defaultProjectPath = "../../Projects/";
    };


    struct ProjectConfig {
        int projectVersion;
    };


    enum class ProjectManagerError : int{
        Unknown = -1,
        Failed_To_Create_Base_Folder = -2,
        Failed_To_Create_Asset_Folder = -3,

        Project_Incompatible_With_Editor = -4,
        Failed_To_Open_Project = -5,

        Success = 0
    };

    class ProjectManager {

    public:

        void init(ProjectManagerCreateInfo* createInfo);

        bool doesProjectExist(const char* projectName);
        ProjectManagerError createProject(const char* projectName, bool auto_open=true);
        ProjectManagerError openProject(const char* projectName);

        void destroy();
        const char* getErrorMessage(ProjectManagerError errorCode);
        void copyFileToProjectAssetFolder(const char* filePath, const char* fileName);

        const char* getProjectPath() { return _currentProject.c_str(); };

        ProjectConfig _config{-1};
    private:


        bool doesPathExist(const char* projectName);
        const char* _defaultProjPath;
        unsigned int _version;

        unsigned int _minCompatibleVer;

        bool _isProjectOpen = false;

        std::string _currentProject;

        ProjectConfig _readingProjectConfig{-1};

    };

}



#endif //GAME_PROJECTMANAGER_H
