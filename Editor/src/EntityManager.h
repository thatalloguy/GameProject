//
// Created by allos on 7/5/2024.
//

#ifndef DUCKWATCHERS_ENTITYMANAGER_H
#define DUCKWATCHERS_ENTITYMANAGER_H

#include <vector>
#include "ProjectManager.h"
#include "Objects/Entity.h"
#include "AssetManager.h"

namespace Lake {

    /*
     * NOTE: The EntityManager actually never creates a "real" entity but only uses the
     * Entity blueprint struct, since that is dynamic and editable.
     * Also the EntityBlueprint struct does not depend on the Jolt library keeping deps to minimum.
     *
     */
    class EntityManager {

    public:

        void Initialize(ProjectManager* projectManager, Lake::AssetManager* assetManager);

        void loadData();
        void exportData();

        void newEntity();
        void deleteEntity(unsigned int ID);
        void selectEntity(unsigned int ID);

        void renderEntityTree();
        void renderEntityInfo(float winWidth);


    private:
        ProjectManager* _projectManager;
        AssetManager* _assetManager;
        std::unordered_map<unsigned int, Quack::EntityBlueprint> _entities;
        int _currentEntityID = -1;

        unsigned int _idCounter = 0;

        char* _tempName = nullptr;

        void renderVector3(Quack::Math::Vector3& vector, float windowWidth, int id);

    };

}

#endif //DUCKWATCHERS_ENTITYMANAGER_H
