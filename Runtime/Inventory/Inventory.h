//
// Created by allos on 6/14/2024.
//

#ifndef MAX_INVENTORY_SIZE
#define MAX_INVENTORY_SIZE 8
#endif



#ifndef GAME_INVENTORY_H
#define GAME_INVENTORY_H



class Page {

public:

    Page() {
        init();
    }
    ~Page() {
        destroy();
    }

    virtual void init() {};
    virtual void destroy() {};

    virtual void renderLeftPage() {};
    virtual void renderRightPage() {  };

    virtual const char* getName() { return "UNKNOWN"; };
};


namespace Inventory {

    class Item {

    };


    struct InventoryItem {

        const char* name;
        unsigned int itemID;

        void* itemData;
    };

    struct Pair {
        unsigned int key;
        Item val[MAX_INVENTORY_SIZE];
    };

    struct Slot {
        unsigned int inventoryKey;
        unsigned int inventoryPos; // inventory[inventoryKey][inventoryPos] == Item();
    };


    // Items And Fish.
    static Pair inventory[2];



}


#endif //GAME_INVENTORY_H
