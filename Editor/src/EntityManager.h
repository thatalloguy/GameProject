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

    typedef unsigned int EntityIndex;
    typedef unsigned int EntityVersion;
    typedef unsigned long long EntityID;

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

        void renderEntitiesToScreen(VulkanEngine& engine);


    private:
        ProjectManager* _projectManager;
        AssetManager* _assetManager;
        std::vector<Quack::EntityBlueprint> _entities;
        int _currentEntityID = -1;
        Quack::EntityBlueprint* _currentEntity;

        unsigned int _idCounter = 0;


        void renderVector3(Quack::Math::Vector3& vector, float windowWidth, int id);


    };

}

#endif //DUCKWATCHERS_ENTITYMANAGER_H
