//
// Created by allos on 8/3/2024.
//

#include "MapLoader.h"



Loader::MapLoader::MapLoader(VulkanEngine &renderer, Quack::PhysicsEngine &physicsEngine) : _renderer(renderer), _physicsEngine(physicsEngine) {

}

Loader::MapLoader::~MapLoader() {
    for (auto entity : _instances) {
        delete entity;
    }
}

void Loader::MapLoader::loadMap() {

    loadAssets();
    loadBlueprints();
    loadInstances();

}

void Loader::MapLoader::renderMap() {
    for (auto entity : _instances) {
        entity->render(_renderer);
    }
}

void Loader::MapLoader::loadAssets() {

    ///renderer.loadedScenes.clear();

    auto json = simdjson::padded_string::load("../../Level.json");


    doc = parser.iterate(json);

    for (auto asset : doc["assets"]) {

        unsigned int ID =  uint64_t(asset["id"]);
        auto path = std::string_view(asset["path"]);


        if (!_renderer.loadedScenes.contains(ID)) {
            auto file = VkLoader::loadGltf(&_renderer, path);
            _renderer.loadedScenes[ID] = *file;
        } else {
            spdlog::warn("CANNOT OVERWRITE ALREADY LOADED ASSETS PLEASE RESTART TO OVERWRITE. ID: {}", ID);
        }
    }
}

void Loader::MapLoader::loadBlueprints() {
    _blueprints.clear();

    auto json = simdjson::padded_string::load("../../Level.json");


    doc = parser.iterate(json);

    for (auto entity : doc["blueprints"]) {
        try {
            Quack::EntityBlueprint blueprint;

            blueprint.ID = (uint64_t) entity["ID"];
            blueprint.name = simdjson::to_json_string(entity["name"]).value();
            blueprint.model = (uint64_t) entity["model"];
            blueprint.isPhysical = entity["isPhysical"];

            // It means the entity blueprints has physics data so we should look for it.
            if (blueprint.isPhysical) {
                auto bodyInfo = entity["Body"];
                //NOTE: shapetype, 0 = box, 1 = sphere, 2 = capsule
                blueprint.shapeType = (Quack::ShapeType) (uint64_t) bodyInfo["shapeType"];
                blueprint.shapeVolume.x = (float) (double) bodyInfo["shapeVolume"]["x"];
                blueprint.shapeVolume.y = (float) (double) bodyInfo["shapeVolume"]["y"];
                blueprint.shapeVolume.z = (float) (double) bodyInfo["shapeVolume"]["z"];

                blueprint.physicsType = (int) (int64_t) bodyInfo["physicsType"];
                blueprint.shouldActivate = bodyInfo["shouldActivate"];
                blueprint.objectLayer = (uint64_t) bodyInfo["objectLayer"];
            }



            spdlog::info("loaded entity: {}", blueprint.name);
            _blueprints.push_back(blueprint);
        } catch (...) {
            spdlog::error("Could not load Entity :( ");
        }
    }
}

void Loader::MapLoader::loadInstances() {

    FILE* f_in = fopen("../../instances.lake", "r");

    EntityInstance temp{};

    while (fread(&temp, sizeof(EntityInstance), 1, f_in) == 1) {

        // lets hope everything works :)
        createEntityFromInstance(temp, *getBlueprintFromID(temp.ID));

        spdlog::debug("loaded instance: {}", temp.ID);
    }

    spdlog::info("loaded instances from file");

    fclose(f_in);
}

Quack::EntityBlueprint* Loader::MapLoader::getBlueprintFromID(unsigned int ID) {
    // Yes i really do a loop to look for the id, boohoo
    for (auto &blueprint : _blueprints) {
        if (blueprint.ID == ID) {
            return &blueprint;
        }
    }
    spdlog::error("Could not find blueprint with ID: {}", ID);
    return nullptr;
}

void Loader::MapLoader::createEntityFromInstance(Loader::EntityInstance &instance, Quack::EntityBlueprint &blueprint)  {
    Quack::EntityCreationInfo creationInfo{};

    creationInfo.position = instance.position;
    creationInfo.size = instance.size;
    creationInfo.rotation = instance.rotation;

    creationInfo.model = blueprint.model;
    creationInfo.isPhysical = blueprint.isPhysical;
    auto& body =creationInfo.bodyCreationInfo;

    body.position.SetX(creationInfo.position.x);
    body.position.SetY(creationInfo.position.y);
    body.position.SetZ(creationInfo.position.z);

    // :)
    _instances.push_back(new Quack::Entity(creationInfo));

    _instances.back()->ID = blueprint.ID;
}
