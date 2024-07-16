//
// Created by allos on 7/15/2024.
//

#include "App.h"
#include "spdlog/spdlog.h"
#include "Video/Window.h"
#include "Objects/Entity.h"
#include "imgui.h"
#include <simdjson.h>

namespace  {

    struct EntityInstance {
        unsigned int ID = 0;
        Quack::Math::Vector3 position{0, 0, 0};
        Quack::Math::Vector3 size{1, 1, 1};
        Quack::Math::Vector4 rotation{0, 0, 0, 1};
    };

    simdjson::ondemand::document doc;
    simdjson::ondemand::parser parser;


    Quack::Window* window;
    VulkanEngine renderer;

    std::vector<Quack::EntityBlueprint> _blueprints;
    std::vector<Quack::Entity*> _instances;
    int selectedIndex = -1;

    void createEntityFromBlueprint(Quack::EntityBlueprint& blueprint) {

        Quack::EntityCreationInfo creationInfo{};

        creationInfo.position.z = -5;

        creationInfo.model = blueprint.model;
        creationInfo.isPhysical = blueprint.isPhysical;
        auto& body =creationInfo.bodyCreationInfo;

        body.position.SetX(creationInfo.position.x);
        body.position.SetY(creationInfo.position.y);
        body.position.SetZ(creationInfo.position.z);

        // :)
        _instances.push_back(new Quack::Entity(creationInfo));


    }

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

                    blueprint.physicsType = (int64_t) bodyInfo["physicsType"];
                    blueprint.shouldActivate = bodyInfo["shouldActivate"];
                    blueprint.objectLayer = (uint64_t) bodyInfo["objectLayer"];
                }



                spdlog::info("loaded entity: {}", blueprint.name);
                _blueprints.push_back(blueprint);
            } catch (...) {
                spdlog::error("Could not load Entity :(");
            }
        }
    }

    void saveInstancesToFile() {

        FILE* f_out = fopen("../../Instances.lake", "w");



        fclose(f_out);

        for (auto instance : _instances) {
            EntityInstance entityInstance{};
            entityInstance.ID = instance->ID;
            entityInstance.position = instance->position;
            entityInstance.size = instance->size;

            fwrite(&entityInstance, sizeof(EntityInstance), 1, f_out);


        }

        fclose(f_out);

    }
    void loadInstancesFromFile() {

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


    renderer.uiRenderFuncs.pushFunction([=](){

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::Begin("Editor", NULL, ImGuiWindowFlags_NoMove);

        if (ImGui::Button("New Entity")) {
            ImGui::OpenPopup("Create new Entity");
        }

        if (ImGui::Button("Save")) {

        } ImGui::SameLine();
        if (ImGui::Button("Load")) {

        }

        if (ImGui::Button("Reload Blueprints")) {

        }


        if (ImGui::BeginPopupModal("Create new Entity")) {

            ImGui::Text("Select Template");

            if (ImGui::BeginCombo(" s", "Choose...")) {
                int i = 0;
                for (auto blueprint : _blueprints) {
                    if (ImGui::Selectable(blueprint.name.c_str())) {
                        selectedIndex = i;
                    }
                    i++;
                }

                ImGui::EndCombo();
            }

            ImGui::Text("Current Selected: %i", selectedIndex);


            if (ImGui::Button("Done")) {
                createEntityFromBlueprint(_blueprints[selectedIndex]);
                ImGui::CloseCurrentPopup();
            } ImGui::SameLine();

            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::End();

    });

    while (!window->shouldShutdown()) {

        renderer.updateScene();

        for (auto entity : _instances) {
            entity->render(renderer);
        }

        renderer.Run();

        window->update();
    }
}

void Lake::App::CleanUp() {

    for (auto entity : _instances) {
        delete entity;
    }

    renderer.CleanUp();

    delete window;
}
