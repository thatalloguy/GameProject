//
// Created by allos on 7/5/2024.
//

#include "EntityManager.h"
#include "IconsFontAwesome6.h"
#include "imgui.h"

#include <filesystem>


void Lake::EntityManager::Initialize(Lake::ProjectManager *projectManager) {

    _projectManager = projectManager;

}

void Lake::EntityManager::exportData() {

}
void Lake::EntityManager::loadData() {

}

void Lake::EntityManager::newEntity() {

    _idCounter++;

    Quack::EntityBlueprint newEntity{.id = _idCounter };

    _entities.emplace(_idCounter, newEntity);
}
void Lake::EntityManager::deleteEntity(unsigned int ID) {
    _entities.erase(ID);
}
void Lake::EntityManager::selectEntity(unsigned int ID) {
    _currentEntityID = ID;
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



        ImGui::BulletText(ICON_FA_USER "    %s", _entities[i].name);
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left)) {
            if (_currentEntityID == entity.id) {
                _currentEntityID = -1;
            } else {
                _currentEntityID = (int) entity.id;
            }
        }

        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

    }
    ImGui::Separator();
    ImGui::EndChild();
}
void Lake::EntityManager::renderEntityInfo() {
    if (_currentEntityID >= 0) {

        auto entity = _entities.at(_currentEntityID);

        ImGui::Text("ID: %i", entity.id);


        ImGui::Text("Name: "); ImGui::SameLine(); ImGui::InputText(" n", *&entity.name, 50);
        // :)
    }
}

