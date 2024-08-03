//
// Created by allos on 8/3/2024.
//

#ifndef DUCKWATCHERS_MAPLOADER_H
#define DUCKWATCHERS_MAPLOADER_H


#include "Math/Vecs.h"
#include "Objects/Entity.h"
#include <simdjson.h>

namespace Loader {
    struct EntityInstance {
        unsigned int ID = 0;
        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 size{1, 1, 1};
        // in degrees
        Quack::Math::Vector3 rotation{0, 0, 0};
    };


    class MapLoader {
    public:
        MapLoader(VulkanEngine& renderer, Quack::PhysicsEngine& physicsEngine);
        ~MapLoader();

        void loadMap();

        void renderMap();
    private:
        void createEntityFromInstance(EntityInstance& instance, Quack::EntityBlueprint& blueprint);

        void loadAssets();
        void loadBlueprints();
        void loadInstances();

        Quack::EntityBlueprint* getBlueprintFromID(unsigned int ID);

        VulkanEngine& _renderer;
        Quack::PhysicsEngine& _physicsEngine;

        std::vector<Quack::EntityBlueprint> _blueprints;
        std::vector<Quack::Entity*> _instances;

        unsigned int DebugModelId = 0;

        simdjson::ondemand::document doc;
        simdjson::ondemand::parser parser;
    };


}



#endif //DUCKWATCHERS_MAPLOADER_H
