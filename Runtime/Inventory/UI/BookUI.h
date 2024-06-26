//
// Created by allos on 6/14/2024.
//

#ifndef GAME_BOOKUI_H
#define GAME_BOOKUI_H

#include <Renderer/VkEngine.h>
#include "Math/Vecs.h"

struct Page;

class BookUI {

public:

    explicit BookUI(VulkanEngine& renderer);
    ~BookUI();

    [[nodiscard]] Quack::Math::Vector2 getUISize() const;

    void shouldRender(bool should);
    bool isRendering() const;

private:

    void registerInternalPages();

    void render();

    //AFAIK no need to use vector :shrug:
    Page* pages[4] = {nullptr, nullptr, nullptr, nullptr};

    VulkanEngine& _renderer;

    bool renderUI = false;

};


#endif //GAME_BOOKUI_H
