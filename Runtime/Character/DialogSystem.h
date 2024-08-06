//
// Created by allos on 8/6/2024.
//

#pragma once


#include <string>

struct DialogBox {
    const char* author = " ";
    std::string dialog = " ";
    DialogBox* next = nullptr;
};

template<auto S>
struct Conversation {
    DialogBox dialogs[S];
};

class DialogRenderer {

public:

    void render();

    void skip(bool bypassToggleLock=false);

    void setCurrentDialog(DialogBox* dialogBox);

};