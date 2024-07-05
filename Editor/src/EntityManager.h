//
// Created by allos on 7/5/2024.
//

#ifndef DUCKWATCHERS_ENTITYMANAGER_H
#define DUCKWATCHERS_ENTITYMANAGER_H

#include <vector>
#include "ProjectManager.h"
#include "Objects/Entity.h"

namespace Lake {

    /*
     * NOTE: The EntityManager actually never creates a "real" entity but only uses the
     * Entity blueprint struct, since that is dynamic and editable.
     * Also the EntityBlueprint struct does not depend on the Jolt library keeping deps to minimum.
     *
     */
    class EntityManager {

    public:

        void Initialize(ProjectManager* projectManager);

        void loadData();
        void exportData();

        unsigned int newEntity();
        void deleteEntity(unsigned int ID);
        void selectEntity(unsigned int ID);

        void renderEntityTree();
        void renderEntityInfo();


    private:
        ProjectManager* _projectManager;
    };

}

#endif //DUCKWATCHERS_ENTITYMANAGER_H
