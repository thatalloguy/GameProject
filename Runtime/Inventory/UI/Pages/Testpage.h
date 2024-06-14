//
// Created by allos on 6/14/2024.
//

#ifndef GAME_TESTPAGE_H
#define GAME_TESTPAGE_H


#include "../../Inventory.h"

#include <imgui.h>


class Testpage : public Page {

public:

    void init() override {
        printf("Hello world from testPage\n");
    }

    void destroy() override {

    }

    const char * getName() override {
        return "TestPage";
    }

    void renderLeftPage() override {
        //ImGui::Text("PAGE");
    }

    void renderRightPage() override {

    }

};


#endif //GAME_TESTPAGE_H
