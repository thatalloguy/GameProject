//
// Created by allos on 6/14/2024.
//

#ifndef MAX_INVENTORY_SIZE
#define MAX_INVENTORY_SIZE 8
#endif

#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H

namespace Inventory {

    template<typename K, typename V>
    struct Pair {
        K key;
        V val;
    };


    struct InventoryItem {

        const char* name;
        unsigned int itemID;

        void* itemData;
    };


    Pair<unsigned int, InventoryItem> inventory[MAX_INVENTORY_SIZE];


}


#endif //GAME_INVENTORY_H
