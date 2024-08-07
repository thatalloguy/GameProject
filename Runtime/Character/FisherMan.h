//
// Created by allos on 8/7/2024.
//

#ifndef DUCKWATCHERS_FISHERMAN_H
#define DUCKWATCHERS_FISHERMAN_H

#include "Objects/Entity.h"
#include "../Core/FishingManager.h"
#include "imgui.h"

namespace Characters {

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

        void shopStart(Player& player);
        void shopEnd(Player& player);


    };



}

#endif //DUCKWATCHERS_FISHERMAN_H
