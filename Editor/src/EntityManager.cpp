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
    _tempName = _entities[ID].name;
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
                selectEntity(entity.id);
            }
        }

        style.Colors[ImGuiCol_Text] = ImVec4(1, 1, 1, 1);

    }
    ImGui::Separator();
    ImGui::EndChild();
}
void Lake::EntityManager::renderEntityInfo(float winWidth) {
    if (_currentEntityID >= 0) {

        auto& entity = _entities.at(_currentEntityID);

        ImGui::BeginDisabled();
        ImGui::Text(ICON_FA_INFO " ID: %i", entity.id);
        ImGui::EndDisabled();


        ImGui::Text(ICON_FA_ID_BADGE" Name: "); ImGui::SameLine(); ImGui::PushID(103); ImGui::InputText(" ", entity.name, 50); ImGui::PopID();
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

        ImGui::Text(ICON_FA_PERSON_WALKING);
        ImGui::SameLine();
        ImGui::Checkbox(" Is Dynamic", &entity.isDynamic);

        ImGui::Spacing();

        ImGui::Separator();

        ImGui::Spacing();

        ImGui::Text(ICON_FA_WEIGHT_SCALE);
        ImGui::SameLine();
        ImGui::Checkbox(" Is Physical", &entity.isPhysical);

        if (entity.isPhysical) {
            if (ImGui::TreeNodeEx(ICON_FA_WEIGHT_HANGING " Physics info", ImGuiTreeNodeFlags_Framed)) {

                ImGui::Combo(ICON_FA_BOX" ShapeType", reinterpret_cast<int *>(&entity.shapeType), "Box\0Sphere\0Capsule\0");

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

