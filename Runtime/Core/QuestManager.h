//
// Created by allos on 8/11/2024.
//

#ifndef DUCKWATCHERS_QUESTMANAGER_H
#define DUCKWATCHERS_QUESTMANAGER_H

#include <functional>
#include "imgui.h"

struct Quest {
    const char* desc;
    std::function<bool()>& condition;
    std::function<void()>& onComplete;
    Quest* next = nullptr;
};

namespace QuestManager {

    void setCurrentQuest(Quest* quest);

    void update();

    void renderUI(ImVec2& windowSize);

}

#endif //DUCKWATCHERS_QUESTMANAGER_H
