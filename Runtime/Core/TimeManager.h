//
// Created by allos on 8/4/2024.
//

#ifndef DUCKWATCHERS_TIMEMANAGER_H
#define DUCKWATCHERS_TIMEMANAGER_H

#include <chrono>

enum class Day : unsigned int {
    Mon = 1,
    Tue = 2,
    Wen = 3,
    Thu = 4,
    Fri = 5,
    Sat = 6,
    Sun = 7
};

class TimeManager {

public:
    TimeManager() = default;
    ~TimeManager() = default;

    void start();
    void tick();

    Day getCurrentDay();
    int getHour(); // from 0 to 23

    void setDay(Day day);
    void setHour(int hour);

private:

    Day _curDay = Day::Mon;
    int _curHour = 12; // The game starts at monday 12 pm

    std::chrono::time_point<std::chrono::steady_clock> _start;
};


#endif //DUCKWATCHERS_TIMEMANAGER_H
