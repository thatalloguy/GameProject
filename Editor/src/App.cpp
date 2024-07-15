//
// Created by allos on 7/15/2024.
//

#include "App.h"
#include "spdlog/spdlog.h"
#include "Video/Window.h"
#include "Objects/Entity.h"
#include <simdjson.h>

namespace  {
    simdjson::ondemand::document doc;
    simdjson::ondemand::parser parser;


    Quack::Window* window;
    VulkanEngine renderer;

    std::vector<Quack::EntityBlueprint> _blueprints;


    void loadAssetsFromJson() {

        for (auto asset : doc["assets"]) {

            unsigned int ID =  uint64_t(asset["id"]);
            auto path = std::string_view(asset["path"]);


            renderer.loadedScenes[ID] = *VkLoader::loadGltf(&renderer, path);
        }

    }

    void loadBlueprints() {
        _blueprints.clear();

        for (auto entity : doc["blueprints"]) {

            Quack::EntityBlueprint blueprint;

            blueprint.name = entity["name"];
            blueprint.model = (uint64_t) entity["model"];
            blueprint.isPhysical = entity["isPhysical"];

            // It means the entity blueprints has physics data so we should look for it.
            if (blueprint.isPhysical) {
                auto bodyInfo = entity["Body"];


                //NOTE: shapetype, 0 = box, 1 = sphere, 2 = capsule


            }



            spdlog::info("loaded entity: {}", blueprint.name);


        }
    }
}


void Lake::App::Init() {


    auto json = simdjson::padded_string::load("../../Level.json");


    doc = parser.iterate(json);


    auto windowinfo = Quack::WindowCreationData{};

    window = new Quack::Window(windowinfo);

    renderer.Init(window->getRawWindow());


    loadAssetsFromJson();

    loadBlueprints();
}

void Lake::App::Run() {


    while (!window->shouldShutdown()) {

        renderer.Run();

        window->update();
    }
}

void Lake::App::CleanUp() {

    renderer.CleanUp();

    delete window;
}
