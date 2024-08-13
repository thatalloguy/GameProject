//
// Created by allos on 8/11/2024.
//

#include "QuestManager.h"

namespace {
    Quest* _currentQuest = nullptr;
}

void QuestManager::setCurrentQuest(Quest* quest) {
    _currentQuest = quest;
}

void QuestManager::update() {

    //nullptr check
    if (_currentQuest) {
        if (_currentQuest->condition()) {
            _currentQuest->onComplete();
            _currentQuest = _currentQuest->next;
        }
    }

}

void QuestManager::renderUI(ImVec2 &windowSize) {
    auto drawList = ImGui::GetForegroundDrawList();


    if (_currentQuest) {
        ImGui::SetWindowFontScale(3.0f);
        drawList->AddText({0, windowSize.y - 100}, ImColor(255, 255, 255), _currentQuest->desc);
        ImGui::SetWindowFontScale(1.0f);
    }
}
