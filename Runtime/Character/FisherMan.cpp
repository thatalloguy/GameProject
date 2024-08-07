//
// Created by allos on 8/7/2024.
//

#include "FisherMan.h"

void Characters::FisherMan::initialize(Quack::Entity &fisherManBaseEntity) {

    _entity = &fisherManBaseEntity;

    setupTrigger();
}

Characters::FisherMan::~FisherMan() {
    delete _trigger;
}

void Characters::FisherMan::setupTrigger() {
    Quack::EntityCreationInfo triggerInfo = {
            .position {_entity->position.x, _entity->position.y - 3, _entity->position.z - 6},
            .size = {3, 8, 5}
    };

    _trigger = new Quack::Entity(triggerInfo);
}

void Characters::FisherMan::update(Player &player) {

    if (_trigger->hasHit(player.position) && player.state == PlayerState::Moving) {
        if (Quack::Input::isControllerPresent(0)) {
            if (Quack::Input::isButtonPressed(0, 0)) {
                spdlog::info("Fisherman Interaction");
                shopStart(player);
            }
        } else {
            if (Quack::Input::isKeyPressed(Quack::Key::E)) {
                spdlog::info("Fisherman Interaction");
                shopStart(player);
            }
        }
    }
}

void Characters::FisherMan::drawUI(ImVec2 windowSize, Player& player) {

    auto drawList = ImGui::GetForegroundDrawList();

    if (player.state == PlayerState::Shopping) {
        ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.3f, windowSize.y * 0.6f));
        ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.4f, windowSize.y * 0.3f));
        ImGui::Begin("Bob the FisherMan", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);

        ImGui::SetWindowFontScale(sqrt((windowSize.x * windowSize.x) + (windowSize.y * windowSize.y)) / 850);
        ImGui::Text("What would ya like?");

        ImGui::Text(" ");
        ImGui::Text(" ");
        ImGui::Text(" ");
        ImGui::Text(" ");
        ImGui::Button("Sell Fish");
        ImGui::SameLine();
        ImGui::Button("Upgrade Fishing Rod");
        ImGui::SameLine();

        ImGui::Button("Bye");
        ImGui::SetWindowFontScale(1.0f);

        ImGui::End();
    }
}

void Characters::FisherMan::shopStart(Player& player) {
    player.state = PlayerState::Shopping;
    player._camera.yaw = -3.2f;
    player._camera.pitch = 0.3f;

    player.position.x = -16;
    player.position.z = 204;
    Quack::Input::setMouseMode(Quack::MouseMode::Normal);
}

void Characters::FisherMan::shopEnd(Player& player) {
    player.state = PlayerState::Moving;
    Quack::Input::setMouseMode(Quack::MouseMode::Disabled);
}
