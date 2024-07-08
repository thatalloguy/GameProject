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
    Quack::EntityBlueprint newEntity{};

    _entities.push_back(newEntity);
}
void Lake::EntityManager::deleteEntity(unsigned int ID) {
    _entities.erase(_entities.begin() + ID);
}
void Lake::EntityManager::selectEntity(unsigned int ID) {
    _currentEntity = &_entities.at(ID);
}


void Lake::EntityManager::renderEntityTree() {

    ImGui::Spacing();

    ImGui::BeginChild("Entity Tree");
    for (int i=0; i<_entities.size(); i++) {
        auto entity =  _entities[i];
        auto& style = ImGui::GetStyle();


        ImGui::Separator();
        if (_currentEntity) {
            if (_currentEntity->id == entity.id) {
                style.Colors[ImGuiCol_Text] = ImVec4(0.5, 0.5, 1, 1);
            }
        }


        ImGui::BulletText(ICON_FA_USER "    %s", _entities[i].name);

        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

    }
    ImGui::Separator();
    ImGui::EndChild();
}
void Lake::EntityManager::renderEntityInfo() {

}

