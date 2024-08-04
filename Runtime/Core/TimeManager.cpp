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
    _starts = std::chrono::high_resolution_clock::now();
}

void TimeManager::tick() {

    auto now = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::minutes>(now - _start);
    auto durations = std::chrono::duration_cast<std::chrono::seconds>(now - _starts);

    if (duration.count() >= 1) {

        _curHour++;

        _start = now;
    }

    if (durations.count() >= 1) {
        _starts = now;

        if (_curHour >= 24) {
            _curDay = static_cast<Day>(static_cast<unsigned int>(_curDay) + 1);
            _curHour = 0;
        }

        // ugly as hell but idc about that anymore, just gotta get this done.

        if (_curHour >= 0 && _curHour < 6) {
            auto v = night_to_morning.step(1);
            skyCol.x = v[0];
            skyCol.y = v[1];
            skyCol.z = v[2];
        }

        if (_curHour >= 6 && _curHour < 12) {
            auto v = morning_to_noon.step(1);
            skyCol.x = v[0];
            skyCol.y = v[1];
            skyCol.z = v[2];
        }

        if (_curHour >= 12 && _curHour < 18) {
            auto v = noon_to_evening.step(1);
            skyCol.x = v[0];
            skyCol.y = v[1];
            skyCol.z = v[2];
        }

        if (_curHour >= 18 && _curHour < 24) {
            auto v = evening_to_night.step(1);
            skyCol.x = v[0];
            skyCol.y = v[1];
            skyCol.z = v[2];
        }

        spdlog::info("Tich");
    }



}


