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

    }

}

void Characters::DebtCollector::drawUI(ImVec2 windowSize, Player &player) {

}
