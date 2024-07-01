//
// Created by allos on 7/1/2024.
//


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
        const char* defaultProjectPath = "../Projects/";
    };

    class ProjectManager {

    public:

        void init(ProjectManagerCreateInfo* createInfo);

        bool doesProjectExist(const char* projectName);
        void createProject(const char* projectName);
        void openProject(const char* projectName);

        void destroy();



    private:
        const char* defaultProjPath;
        unsigned int version;
        unsigned int minCompatibleVer;

    };

}



#endif //GAME_PROJECTMANAGER_H
