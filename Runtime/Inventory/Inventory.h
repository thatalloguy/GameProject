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




    struct InventoryItem {

        const char* name;
        unsigned int itemID;

        void* itemData;
    };

    struct Pair {
        unsigned int key;
        InventoryItem val;
    };


    static Pair inventory[MAX_INVENTORY_SIZE];


}


#endif //GAME_INVENTORY_H
