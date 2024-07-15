//
// Created by allos on 7/15/2024.
//

#include "App.h"
#include "spdlog/spdlog.h"
#include "Video/Window.h"
#include <simdjson.h>

namespace  {
    simdjson::ondemand::document doc;
    simdjson::ondemand::parser parser;


    Quack::Window* window;
    VulkanEngine renderer;


    void loadAssetsFromJson() {

        for (auto asset : doc["assets"]) {

            unsigned int ID =  uint64_t(asset["id"]);
            auto path = std::string_view(asset["path"]);


            renderer.loadedScenes[ID] = *VkLoader::loadGltf(&renderer, path);
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
