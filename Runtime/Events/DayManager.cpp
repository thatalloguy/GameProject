//
// Created by allos on 8/9/2024.
//

#include "DayManager.h"

DayManager::DayManager(TimeManager &timeManager, VulkanEngine &renderer) : _timeManager(timeManager), _renderer(renderer) {


    //todo load entities.
}

void DayManager::update() {

    switch (_timeManager.getCurrentDay()) {
        case Day::Mon:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::Tue:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::Wen:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::Thu:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::Fri:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::Sat:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::Sun:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
        case Day::End:
            if (_timeManager.getHour() <= 20) {
                // day
            } else {
                // night
            }
            break;
    }

}
