//
// Created by allos on 8/9/2024.
//

#include "DebtCollector.h"

Characters::DebtCollector::~DebtCollector() {

}

void Characters::DebtCollector::initialize(Quack::Entity &baseEntity, TimeManager &timeManager) {
    _entity = &baseEntity;
    _timeManager = &timeManager;
}

void Characters::DebtCollector::update(Player &player) {

    if (_timeManager->getCurrentDay() == Day::Mon) {
        if (_currentState == CharacterState::Opening) {
            spdlog::info("Yapping");
            _entity->position.y = 20.5f;
            DialogRenderer::setCurrentConversation(&opening);

            _currentState = CharacterState::Talking;
            player.state = PlayerState::Cutscene;
        }
    }


    if (_timeManager->getCurrentDay() == Day::Wen && _timeManager->getHour() >= 7 && !hasYapped) {
        _currentState = CharacterState::Wensday;
        hasYapped = true;
    }
    if (_currentState == CharacterState::Wensday) {
            spdlog::info("Yapping The sequel");
            _entity->position.y = 20.5f;
            if (player.sleepingCounter == 1) {
                DialogRenderer::setCurrentConversation(&reminder_3);
            } else {
                if (player.inventory.money >= 100) {
                    DialogRenderer::setCurrentConversation(&reminder_2);
                } else {
                    DialogRenderer::setCurrentConversation(&reminder_1);
                }
            }

            _currentState = CharacterState::Talking;
            player.state = PlayerState::Cutscene;
            player._character->SetPosition({3, -0.2f, 33});
            player._camera.pitch = 0.0f;
            player._camera.yaw = 0.0f;
    }


    if (_currentState == CharacterState::Talking && DialogRenderer::isCurrentConvEmpty()) {
        _currentState = CharacterState::Disabled;
        player.state = PlayerState::Moving;
    }
    if (_currentState == CharacterState::Disabled) {
        _entity->position.y = -50;
    } else {
        _entity->position.y = 4.5f;
        _entity->position.x = 1.0f;
        _entity->position.z = 26.0f;
    }

}

void Characters::DebtCollector::drawUI(ImVec2 windowSize, Player &player) {

}
