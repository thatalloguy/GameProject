//
// Created by allos on 7/5/2024.
//

#ifndef DUCKWATCHERS_LEVELMANAGER_H
#define DUCKWATCHERS_LEVELMANAGER_H

#include <vector>
#include "ProjectManager.h"
#include "Objects/Level.h"

namespace Lake {

    /*
     * NOTE: The LevelManager actually never creates a "real" entity but only uses the
     * Entity blueprint struct, since that is dynamic and editable.
     * Also the EntityBlueprint struct does not depend on the Jolt library keeping deps to minimum.
     *
     */
    class LevelManager {

    public:

        void Initialize(ProjectManager* projectManager, bool shouldLoadLevels=true);

        void newLevel(const char* name);
        void selectLevel(const char* name);

        void loadLevel(const char* fileName, const char* name);
        void exportData();

        unsigned int newEntity();

        void deleteEntity(unsigned int ID);

        void selectEntity(unsigned int ID);

        void renderEntityTree();
        void renderEntityInfo();

        const char* getLevelName() { return _currentLevel->name; };


    private:
        unsigned int _entityCount = 0;
        unsigned int _idCount = 0;

        ProjectManager* _projectManager;

        std::vector<Quack::Level> _levels;
        Quack::Level* _currentLevel = nullptr;
    };

}

#endif //DUCKWATCHERS_LEVELMANAGER_H
