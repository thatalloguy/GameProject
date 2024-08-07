//
// Created by allos on 8/7/2024.
//

#ifndef DUCKWATCHERS_FISHERMAN_H
#define DUCKWATCHERS_FISHERMAN_H

#include "Objects/Entity.h"
#include "../Core/FishingManager.h"

namespace Characters {

    class FisherMan {

    public:

        FisherMan(Quack::Entity& fisherManBaseEntity, Player& player);
        ~FisherMan();

        void update();

    private:

    };



}

#endif //DUCKWATCHERS_FISHERMAN_H
