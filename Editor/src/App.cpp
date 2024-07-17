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
    int selectedIndex = -1; // this one is for selecting blueprints in the popup
    int selectedEntityIndex = -1;

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

        _instances.back()->ID = blueprint.ID;

    }
    void createEntityFromInstance(EntityInstance& instance, Quack::EntityBlueprint& blueprint) {
        Quack::EntityCreationInfo creationInfo{};

        creationInfo.position = instance.position;
        creationInfo.size = instance.size;

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

    Quack::EntityBlueprint* getBlueprintFromID(unsigned int ID) {
        // Yes i really do a loop to look for the id, boohoo
        for (auto &blueprint : _blueprints) {
            if (blueprint.ID == ID) {
                return &blueprint;
            }
        }
        spdlog::error("Could not find blueprint with ID: {}", ID);
        return nullptr;
    }

    void loadAssetsFromJson() {

        ///renderer.loadedScenes.clear();

        auto json = simdjson::padded_string::load("../../Level.json");


        doc = parser.iterate(json);

        for (auto asset : doc["assets"]) {

            unsigned int ID =  uint64_t(asset["id"]);
            auto path = std::string_view(asset["path"]);


            if (!renderer.loadedScenes.contains(ID)) {
                renderer.loadedScenes[ID] = *VkLoader::loadGltf(&renderer, path);
            } else {
                spdlog::warn("CANNOT OVERWRITE ALREADY LOADED ASSETS PLEASE RESTART TO OVERWRITE. ID: {}", ID);
            }
        }

    }
    void loadBlueprints() {
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

    void saveInstancesToFile() {

        FILE* f_out = fopen("../../Instances.lake", "w");


        // I hate my past self :(
        //fclose(f_out);

        for (auto& instance : _instances) {

            EntityInstance entityInstance{};
            entityInstance.ID = instance->ID;
            entityInstance.position = instance->position;
            entityInstance.size = instance->size;

            fwrite(&entityInstance, sizeof(EntityInstance), 1, f_out);


            spdlog::debug("Wrote entity to file");
        }

        fclose(f_out);

    }
    void loadInstancesFromFile() {

        FILE* f_in = fopen("../../Instances.lake", "r");

        EntityInstance temp{};

        while (fread(&temp, sizeof(EntityInstance), 1, f_in) == 1) {

            // lets hope everything works :)
            createEntityFromInstance(temp, *getBlueprintFromID(temp.ID));

            spdlog::debug("loaded instance: {}", temp.ID);
        }

        spdlog::info("loaded instances from file");

        fclose(f_in);


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

        int width, height;
        glfwGetWindowSize(window->getRawWindow(), &width, &height);

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float) width * 0.2f,(float) height));
        ImGui::Begin("Editor", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        if (ImGui::Button("New Entity")) {
            ImGui::OpenPopup("Create new Entity");
        }

        if (ImGui::Button("Save")) {
            saveInstancesToFile();
            spdlog::info("Saved Instances to file");
        } ImGui::SameLine();
        if (ImGui::Button("Load")) {
            loadInstancesFromFile();
        }

        if (ImGui::Button("Reload Blueprints")) {
            loadBlueprints();
        }
        if (ImGui::Button("Reload Assets")) {
            loadAssetsFromJson();
        }


        ImGui::SeparatorText("Entities");

        ImGui::BeginChild("Treeview");
        int i = 0;
        for (auto entity : _instances) {
            ImGui::PushID(i);
            if (ImGui::Selectable("Entity ")) {
                if (selectedEntityIndex == i) {
                    //if where already selected, unselect the entity
                    selectedEntityIndex = -1;
                } else {
                    selectedEntityIndex = i;
                }
            } ImGui::PopID(); ImGui::SameLine(); ImGui::Text(" : %i", entity->ID);
            i++;
        }

        ImGui::EndChild();


        if (ImGui::BeginPopupModal("Create new Entity")) {

            ImGui::Text("Select Template");

            if (ImGui::BeginCombo(" s", "Choose...")) {
                int index = 0;
                for (auto& blueprint : _blueprints) {
                    if (ImGui::Selectable(blueprint.name.c_str())) {
                        selectedIndex = index;
                    }
                    index++;
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

        if (selectedEntityIndex > -1) {

            ImGui::Begin("Entity Info");

            auto entity = _instances[selectedEntityIndex];

            ImGui::SeparatorText("Position");
            const float size = 50.0f;
            ImGui::SetNextItemWidth(size);
            ImGui::DragFloat("PX", &entity->position.x, 0.1f); ImGui::SameLine(); ImGui::SetNextItemWidth(size);
            ImGui::DragFloat("PY", &entity->position.y, 0.1f); ImGui::SameLine(); ImGui::SetNextItemWidth(size);
            ImGui::DragFloat("PZ", &entity->position.z, 0.1f);
            ImGui::SeparatorText("Scale");
            ImGui::SetNextItemWidth(size);
            ImGui::DragFloat("SX", &entity->size.x, 0.1f); ImGui::SameLine(); ImGui::SetNextItemWidth(size);
            ImGui::DragFloat("SY", &entity->size.y, 0.1f); ImGui::SameLine(); ImGui::SetNextItemWidth(size);
            ImGui::DragFloat("SZ", &entity->size.z, 0.1f);


            ImGui::End();

        }

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
