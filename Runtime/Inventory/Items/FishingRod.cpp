//
// Created by allos on 6/14/2024.
//

#include "FishingRod.h"

void FishingRod::destroy() {

}

void FishingRod::update() {

}

void FishingRod::init() {

    this->rarity = ItemRarity::Rare;
    this->name = "Fishing Rod";
    this->description = "A handy tool used to catch fish";

    this->ItemID = 1; // fishingrod = 1
}
