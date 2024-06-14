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

    BookUI(VulkanEngine& renderer);
    ~BookUI();

    Quack::Math::Vector2 getUISize() const;

    void shouldRender(bool should);
    bool isRendering();

private:

    void registerInternalPages();

    void render();

    //AFAIK no need to use vector :shrug:
    Page* pages[4] = {nullptr, nullptr, nullptr, nullptr};

    VulkanEngine& _renderer;

    bool renderUI = true;

};


#endif //GAME_BOOKUI_H
