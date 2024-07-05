//
// Created by allos on 7/5/2024.
//

#ifndef DUCKWATCHERS_ENTITYMANAGER_H
#define DUCKWATCHERS_ENTITYMANAGER_H

#include <vector>
#include "ProjectManager.h"
#include "Objects/Level.h"

namespace Lake {

    /*
     * NOTE: The EntityManager actually never creates a "real" entity but only uses the
     * Entity blueprint struct, since that is dynamic and editable.
     * Also the EntityBlueprint struct does not depend on the Jolt library keeping deps to minimum.
     *
     */
    class EntityManager {

    public:

        void Initialize(ProjectManager* projectManager, bool shouldLoadLevels=true);

        void newLevel();

        void loadLevel(const char* fileName);

        void newEntity();

        void deleteEntity(unsigned int ID);

        void selectEntity(unsigned int ID);

        void renderUI(float windowWidth, float windowHeight);

        void exportData();

    private:
        unsigned int _entityCount = 0;
        unsigned int _idCount = 0;

        std::vector<Quack::Level> _levels;

    };

}

#endif //DUCKWATCHERS_ENTITYMANAGER_H
