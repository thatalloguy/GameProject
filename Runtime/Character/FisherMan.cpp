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
        drawList->AddText(ImVec2{500, 500}, ImColor(255, 255, 255), "Shopping");
    }
}

void Characters::FisherMan::shopStart(Player& player) {
    player.state = PlayerState::Shopping;
    player._camera.yaw = -3.2f;
    player._camera.pitch = 0.3f;

    player.position.x = -18;
    player.position.z = 204;
}

void Characters::FisherMan::shopEnd(Player& player) {
    player.state = PlayerState::Moving;
}
