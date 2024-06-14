//
// Created by allos on 6/14/2024.
//

#include "BookUI.h"

#include "Pages/Testpage.h"

BookUI::BookUI(VulkanEngine &renderer) : _renderer(renderer) {
    registerInternalPages();

    _renderer.uiRenderFuncs.pushFunction([&](){
        if (renderUI) {
            this->render();
        }
    });
}

BookUI::~BookUI() {
    for (auto page : pages) {
        if (page != nullptr) {
            delete page;
        }
    }
}

Quack::Math::Vector2 BookUI::getUISize() const {
    return Quack::Math::Vector2(_renderer._drawExtent.width * 0.75f, _renderer._drawExtent.height * 0.75f);
}

void BookUI::registerInternalPages() {
    pages[0] = new Testpage();
    pages[0]->init();
}

void BookUI::render() {
    ImGui::GetForegroundDrawList()->AddRectFilled({50, 50}, {500, 500}, ImColor(255, 255, 255));

    //temp
    pages[0]->renderLeftPage();

}

void BookUI::shouldRender(bool should) {
    renderUI = should;
}

bool BookUI::isRendering() {
    return renderUI;
}
