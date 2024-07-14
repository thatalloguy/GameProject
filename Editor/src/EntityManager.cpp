//
// Created by allos on 7/5/2024.
//

#include "EntityManager.h"
#include "IconsFontAwesome6.h"
#include "imgui.h"

#include <filesystem>

void itemDescription(const char* description) {
    if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayNormal)) { ImGui::SetTooltip(description); }
}

glm::mat4 getTransformMatrix(Quack::EntityBlueprint& entity)  {
    glm::mat4 model{1.f};

    model =  glm::translate(model, glm::vec3{entity.position.x, entity.position.y, entity.position.z});
    model =  glm::scale(model, glm::vec3{entity.size.x, entity.size.y, entity.size.z});

    return model;
}


void Lake::EntityManager::Initialize(Lake::ProjectManager *projectManager, Lake::AssetManager* assetManager) {

    _projectManager = projectManager;
    _assetManager = assetManager;
}

void Lake::EntityManager::exportData() {

}
void Lake::EntityManager::loadData() {

}

void Lake::EntityManager::newEntity() {


    Quack::EntityBlueprint newEntity{};
    newEntity.id = _idCounter;

    _entities.push_back(newEntity);

    _idCounter++;
}
void Lake::EntityManager::deleteEntity(unsigned int ID) {
    for (int i=0; i<_entities.size(); i++) {
        if (ID == _entities[i].id) {
            _entities.erase(_entities.begin() + i);
        }
    }
}
void Lake::EntityManager::selectEntity(unsigned int ID) {
    for (int i=0; i<_entities.size(); i++) {
        if (ID == _entities[i].id) {
            _currentEntityID = ID;
            _currentEntity = &_entities[i];
        }
    }

}


void Lake::EntityManager::renderEntityTree() {

    ImGui::Spacing();

    ImGui::BeginChild("Entity Tree");
    for (int i=0; i<_entities.size(); i++) {
        auto entity =  _entities[i];
        auto& style = ImGui::GetStyle();


        ImGui::Separator();
        if (_currentEntityID == entity.id) {
            style.Colors[ImGuiCol_Text] = ImVec4(0.5, 0.5, 1, 1);
        }



        ImGui::BulletText(ICON_FA_USER "    %s", entity.name);
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            if (_currentEntityID == entity.id) {
                _currentEntityID = -1;
            } else {
                selectEntity(entity.id);
            }
        }

        ImGui::SameLine();
        ImGui::BeginDisabled();
        ImGui::Text("  ID: %i", entity.id);
        ImGui::EndDisabled();

        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

    }
    ImGui::Separator();
    ImGui::EndChild();
}
void Lake::EntityManager::renderEntityInfo(float winWidth) {
    if (_currentEntityID >= 0) {

        auto& entity = *_currentEntity;

        ImGui::BeginDisabled();
        ImGui::Text(ICON_FA_INFO " ID: %i", entity.id);
        ImGui::EndDisabled();

        ImGui::SameLine(); ImGui::Text("   "); ImGui::SameLine();
        if (ImGui::Button(ICON_FA_TRASH)) {
            deleteEntity(entity.id);
            _currentEntityID = -1;
        }

        ImGui::Text(ICON_FA_ID_BADGE" Name: "); ImGui::SameLine(); ImGui::PushID(103); ImGui::InputText(" ", entity.name, 50); ImGui::PopID();
        ImGui::Separator();
        ImGui::Spacing();

        _assetManager->renderAssetSelectionUI(entity);
        ImGui::Spacing();

        ImGui::Separator();
        ImGui::Spacing();

        ImGui::SetNextItemWidth(winWidth * 0.25f);
        if (ImGui::TreeNodeEx(ICON_FA_STREET_VIEW " Transform", ImGuiTreeNodeFlags_Framed)) {
            ImGui::Spacing();
            ImGui::Text(ICON_FA_LOCATION_DOT " Position: ");
            ImGui::SameLine();

            renderVector3(entity.position, winWidth, 1);

            ImGui::Spacing();
            ImGui::Text(ICON_FA_UP_RIGHT_AND_DOWN_LEFT_FROM_CENTER " Size: ");
            ImGui::SameLine();

            renderVector3(entity.size, winWidth, 2);

            ImGui::TreePop();
        }
        ImGui::Separator();

        ImGui::Spacing();


        ImGui::Separator();

        ImGui::Spacing();

        ImGui::Text(ICON_FA_WEIGHT_SCALE);
        ImGui::SameLine();
        ImGui::Checkbox(" Is Physical", &entity.isPhysical);

        if (entity.isPhysical) {

            ImGui::SetNextItemWidth(winWidth * 0.25f);
            if (ImGui::TreeNodeEx(ICON_FA_WEIGHT_HANGING " Physics info", ImGuiTreeNodeFlags_Framed)) {

                ImGui::Text(ICON_FA_FLAG_CHECKERED " Start Active");
                itemDescription("Tell the physics engine that the body should be active on start\n handy for stuff like dynamic objects that move without the player touching them");
                ImGui::SameLine();
                ImGui::Checkbox(" _", &entity.shouldActivate);


                ImGui::Text(ICON_FA_PERSON_WALKING " Physics Type: "); ImGui::SameLine(); ImGui::PushID(148);
                ImGui::SetNextItemWidth(winWidth * 0.075f);
                ImGui::Combo(" ", reinterpret_cast<int *>(&entity.physicsType), "Static\0Kinematic\0Dynamic\0"); ImGui::PopID();


                ImGui::Spacing();
                ImGui::Combo(ICON_FA_BOX" ShapeType", reinterpret_cast<int *>(&entity.shapeType), "Box\0Sphere\0Capsule\0");

                switch (entity.shapeType) {
                    case Quack::ShapeType::Capsule:
                        ImGui::Text("Radius: "); ImGui::SameLine(); ImGui::PushID(95); ImGui::DragFloat(" ", &entity.shapeVolume.x, 0.1f); ImGui::PopID();
                        ImGui::Text("Height: "); ImGui::SameLine(); ImGui::PushID(96); ImGui::DragFloat(" ", &entity.shapeVolume.y, 0.1f); ImGui::PopID();
                        break;

                    case Quack::ShapeType::Box:
                        renderVector3(entity.shapeVolume, winWidth * 0.95f, 9);
                        break;

                    case Quack::ShapeType::Sphere:
                        ImGui::Text("Radius: "); ImGui::SameLine(); ImGui::PushID(95); ImGui::DragFloat(" ", &entity.shapeVolume.x, 0.1f); ImGui::PopID();
                        break;

                    default:
                        break;
                }

                ImGui::TreePop();
            }

        }
    }
}

void Lake::EntityManager::renderVector3(Quack::Math::Vector3 &vector, float windowWidth, int id) {

    //x


    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);

    ImGui::PushID(4 * id);
    ImGui::SetNextItemWidth((windowWidth * 0.25f) * 0.15f);
    ImGui::DragFloat(" ", &vector.x,  0.1f);
    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::SameLine();

    //y
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);

    ImGui::PushID(5 * id);
    ImGui::SetNextItemWidth((windowWidth * 0.25f) * 0.15f);
    ImGui::DragFloat(" ", &vector.y,  0.1f);
    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();

    ImGui::SameLine();

    //y
    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.1f, 0.1f, 0.7f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 2.0f);

    ImGui::PushID(6 * id);
    ImGui::SetNextItemWidth((windowWidth * 0.25f) * 0.15f);
    ImGui::DragFloat(" ", &vector.z,  0.1f);
    ImGui::PopID();

    ImGui::PopStyleVar();
    ImGui::PopStyleColor();
}

void Lake::EntityManager::renderEntitiesToScreen(VulkanEngine &engine) {

    for (auto entity : _entities) {
        if (entity.model > 0) {
            engine.loadedScenes[entity.model]->Draw(getTransformMatrix(entity), engine.mainDrawContext);
        }
    }

}

