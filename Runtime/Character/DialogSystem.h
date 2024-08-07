//
// Created by allos on 8/6/2024.
//

#pragma once


#include <string>
#include <vector>
#include "imgui.h"


struct DialogBox {
    const char* author = " ";
    std::vector<std::string> dialogs;
    DialogBox* next = nullptr;
};

namespace DialogRenderer {

    static float speakingSpeed = 0.2f;

    void render(ImVec2& windowSize);

    void skip(bool bypassToggleLock=false);

    //void setCurrentDialog(DialogBox* dialogBox);
    void setCurrentConversation(DialogBox* dialogBox);

};