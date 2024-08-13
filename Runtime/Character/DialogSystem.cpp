//
// Created by allos on 8/6/2024.
//

#include "DialogSystem.h"
#include <Input/InputManager.h>

namespace {

    DialogBox* _currentDialog = nullptr;
    int currentLine = 0;
    float currentPos = 1.0f;

    bool isDone = false;
    bool isLineDone = false;
}

void DialogRenderer::render(ImVec2& windowSize) {

    if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isDone) {
        currentLine = 0;
        currentPos = 1;
        _currentDialog = _currentDialog->next;
        isDone = false;
        isLineDone = false;
    }


    if (_currentDialog) {
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && isLineDone) {
            currentLine++;
            currentPos = 1;
            isLineDone = false;
        }

        //calc the sentence to be rendererd

        if (currentLine >= _currentDialog->dialogs.size()) {
            isDone = true;
            isLineDone = true;
        } else {
            if (currentPos >= _currentDialog->dialogs[currentLine].size()) {
                isLineDone = true;
            } else {
                currentPos += speakingSpeed;
            }
        }



        std::string finalString;


        if (isDone) {
            finalString = _currentDialog->dialogs.back();
        } else {
            finalString = _currentDialog->dialogs[currentLine].substr(0, (int) currentPos);
        }


        //render that bad boy!
        ImGui::SetNextWindowPos(ImVec2(windowSize.x * 0.3f, windowSize.y * 0.6f));
        ImGui::SetNextWindowSize(ImVec2(windowSize.x * 0.4f, windowSize.y * 0.3f));
        ImGui::Begin(_currentDialog->author, NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoMouseInputs);

        ImGui::SetWindowFontScale(sqrt((windowSize.x * windowSize.x) + (windowSize.y * windowSize.y)) / 850);
        ImGui::Text(finalString.c_str());

        ImGui::Text(" ");
        ImGui::Text(" ");
        ImGui::Text(" ");
        ImGui::Text(" ");
        ImGui::Text(" ");
        if (isLineDone) {
            ImGui::Text("Press Left to continue");
        }
        ImGui::SetWindowFontScale(1.0f);

        ImGui::End();
    }


}

void DialogRenderer::skip(bool bypassToggleLock) {

}

void DialogRenderer::setCurrentConversation(DialogBox *dialogBox) {
    _currentDialog = dialogBox;
}

bool DialogRenderer::isCurrentConvEmpty() {
    return _currentDialog == nullptr;
}
