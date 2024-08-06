//
// Created by allos on 8/4/2024.
//

#ifndef DUCKWATCHERS_TIMEMANAGER_H
#define DUCKWATCHERS_TIMEMANAGER_H

#include <chrono>
#include <tweeny.h>
#include "Math/Vecs.h"

enum class Day : unsigned int {
    Mon = 1,
    Tue = 2,
    Wen = 3,
    Thu = 4,
    Fri = 5,
    Sat = 6,
    Sun = 7,
    End = 8


};

static const char* toCstr(Day day) {
        switch (day) {

            case Day::Mon:
                return "Monday";
            case Day::Tue:
                return "Tuesday";
            case Day::Wen:
                return "Wednesday";
            case Day::Thu:
                return "Thursday";
            case Day::Fri:
                return "Friday";
            case Day::Sat:
                return "Saturday";
            case Day::Sun:
                return "Sunday";
            default:
                return "?End?";

        }
    }


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

    void forcedUpdate();

    //w = sunDirection
    Quack::Math::Vector4 skyCol{0, 0, 0, 1.0f};

private:

    Day _curDay = Day::Mon;
    int _curHour = 12; // The game starts at monday 12 pm

    //tweens for the sky color
    tweeny::tween<float, float, float> morning_to_noon = tweeny::from(0.0f, 0.3f, 0.7f).to(0.0f, 0.9f, 1.0f).during(360);
    tweeny::tween<float, float, float> noon_to_evening = tweeny::from(0.0f, 0.9f, 1.0f).to(0.2f, 0.1f, 0.4f).during(360);
    tweeny::tween<float, float, float> evening_to_night = tweeny::from(0.2f, 0.1f, 0.4f).to(0.0f, 0.1f, 0.2f).during(360);
    tweeny::tween<float, float, float> night_to_morning = tweeny::from(0.0f, 0.1f, 0.2f).to(0.0f, 0.3f, 0.7f).during(360);

    //tweens for the sun direction
    tweeny::tween<float> sun_n_m = tweeny::from(-0.3f).to(0.5f).during(360);
    tweeny::tween<float> sun_m_n = tweeny::from(0.5f).to(1.0f).during(360);
    tweeny::tween<float> sun_n_e = tweeny::from(1.0f).to(0.5f).during(360);
    tweeny::tween<float> sun_e_n = tweeny::from(0.5f).to(-0.3f).during(360);

    std::chrono::time_point<std::chrono::steady_clock> _start;
    std::chrono::time_point<std::chrono::steady_clock> _starts;
};


#endif //DUCKWATCHERS_TIMEMANAGER_H
