//
// Created by allos on 8/9/2024.
//

#include "DebtCollector.h"

Characters::DebtCollector::~DebtCollector() {

}

void Characters::DebtCollector::initialize(Quack::Entity &baseEntity, TimeManager &timeManager) {
    _entity = &baseEntity;
    _timeManager = &timeManager;

    DialogRenderer::setCurrentConversation(&opening);
}

void Characters::DebtCollector::update(Player &player) {

}

void Characters::DebtCollector::drawUI(ImVec2 windowSize, Player &player) {

}
