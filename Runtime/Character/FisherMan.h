//
// Created by allos on 8/7/2024.
//

#ifndef DUCKWATCHERS_FISHERMAN_H
#define DUCKWATCHERS_FISHERMAN_H

#include "Objects/Entity.h"
#include "../Core/FishingManager.h"
#include "imgui.h"

namespace Characters {

    enum class ShopTab : int {
        Dialog = 0,
        Selection = 1,
        Selling = 2,
        Upgrading = 3
    };

    class FisherMan {

    public:

        FisherMan() = default;
        ~FisherMan();

        void initialize(Quack::Entity& fisherManBaseEntity);

        void update(Player& player);

        void drawUI(ImVec2 windowSize, Player& player);

    private:
        Quack::Entity* _entity;
        Quack::Entity* _trigger;


        void setupTrigger();

        ShopTab _currentTab = ShopTab::Selection;

        void shopStart(Player& player);
        void shopEnd(Player& player);


        void drawSellingTab(ImVec2 windowSize, Player& player);
        void drawUpgradingTab(ImVec2 windowSize, Player& player);
        void drawSelectionTab(ImVec2 windowSize, Player& player);
        //todo dialog


    };



}

#endif //DUCKWATCHERS_FISHERMAN_H
