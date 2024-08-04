//
// Created by allos on 8/4/2024.
//

#include "TimeManager.h"
#include "spdlog/spdlog.h"

int TimeManager::getHour() {
    return _curHour;
}

Day TimeManager::getCurrentDay() {
    return _curDay;
}

void TimeManager::setDay(Day day) {
    _curDay = day;
}

void TimeManager::setHour(int hour) {
    _curHour = hour;
}

void TimeManager::start() {
    _start = std::chrono::high_resolution_clock::now();
}

void TimeManager::tick() {

    auto now = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - _start);

    if (duration.count() >= 1) {

        _curHour++;

        _start = now;
    }

    if (_curHour >= 24) {
        _curDay = static_cast<Day>(static_cast<unsigned int>(_curDay) + 1);
        _curHour = 0;
    }


}


